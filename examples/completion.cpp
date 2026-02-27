/**
 * @file completion.cpp
 * @brief Plugin — generate shell completion scripts.
 *
 * Usage:  dev completion <bash|zsh|fish|pwsh>
 */

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <print>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

static std::vector<std::string> discover_plugins()
{
    std::vector<std::string> names;

    // Check cwd/plugins (most common during dev)
    fs::path dir = fs::current_path() / "plugins";
    if (!fs::is_directory(dir))
        return names;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (!entry.is_regular_file())
            continue;
        auto stem = entry.path().stem().string();
        if (stem != "completion") { // don't complete ourselves
            names.push_back(stem);
        }
    }
    std::sort(names.begin(), names.end());
    return names;
}

static std::string join_space(const std::vector<std::string>& v)
{
    std::string s;
    for (const auto& x : v) {
        if (!s.empty())
            s += ' ';
        s += x;
    }
    return s;
}

static void gen_bash(const std::vector<std::string>& plugins)
{
    auto cmds = join_space(plugins);
    std::println("# Bash completion for dev");
    std::println("# Add to ~/.bashrc:  eval \"$(dev completion bash)\"");
    std::println("_dev_completions() {{");
    std::println("  local cur=${{COMP_WORDS[COMP_CWORD]}}");
    std::println("  if [[ $COMP_CWORD -eq 1 ]]; then");
    std::println("    COMPREPLY=($(compgen -W \"{} list help --help --version\" "
                 "-- \"$cur\"))",
                 cmds);
    std::println("  fi");
    std::println("}}");
    std::println("complete -F _dev_completions dev");
}

static void gen_zsh(const std::vector<std::string>& plugins)
{
    std::println("# Zsh completion for dev");
    std::println("# Add to ~/.zshrc:  eval \"$(dev completion zsh)\"");
    std::println("_dev() {{");
    std::println("  local -a commands=(");
    for (const auto& p : plugins) {
        std::println("    '{}'", p);
    }
    std::println("    'list' 'help' '--help' '--version'");
    std::println("  )");
    std::println("  _arguments '1:command:compadd -a commands' '*:file:_files'");
    std::println("}}");
    std::println("compdef _dev dev");
}

static void gen_fish(const std::vector<std::string>& plugins)
{
    std::println("# Fish completion for dev");
    std::println("# Add to ~/.config/fish/completions/dev.fish");
    std::println("complete -c dev -e");
    for (const auto& p : plugins) {
        std::println("complete -c dev -n '__fish_use_subcommand' -a '{}' -d 'Plugin'", p);
    }
    std::println("complete -c dev -n '__fish_use_subcommand' -a 'list' -d 'List "
                 "available commands'");
    std::println("complete -c dev -n '__fish_use_subcommand' -a 'help' -d 'Show "
                 "help for a command'");
    std::println("complete -c dev -n '__fish_use_subcommand' -l 'help' -d 'Show help'");
    std::println("complete -c dev -n '__fish_use_subcommand' -l 'version' -d "
                 "'Show version'");
}

static void gen_pwsh(const std::vector<std::string>& plugins)
{
    std::println("# PowerShell completion for dev");
    std::println("# Add to $PROFILE:  dev completion pwsh | Invoke-Expression");
    std::println("Register-ArgumentCompleter -CommandName dev -Native -ScriptBlock {{");
    std::println("  param($wordToComplete, $commandAst, $cursorPosition)");
    std::println("  $commands = @(");
    for (const auto& p : plugins) {
        std::println("    '{}'", p);
    }
    std::println("    'list'; 'help'; '--help'; '--version'");
    std::println("  )");
    std::println("  $commands | Where-Object {{ $_ -like \"$wordToComplete*\" }} |");
    std::println("    ForEach-Object {{ "
                 "[System.Management.Automation.CompletionResult]::new($_, $_, "
                 "'ParameterValue', $_) }}");
    std::println("}}");
}

int main(int argc, char* argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0) {
        std::println("completion — generate shell completion scripts");
        std::println("");
        std::println("usage: dev completion <shell>");
        std::println("");
        std::println("shells:");
        std::println("  bash   Bash completion");
        std::println("  zsh    Zsh completion");
        std::println("  fish   Fish completion");
        std::println("  pwsh   PowerShell completion");
        std::println("");
        std::println("setup:");
        std::println("  bash:  eval \"$(dev completion bash)\"");
        std::println("  zsh:   eval \"$(dev completion zsh)\"");
        std::println("  fish:  dev completion fish > ~/.config/fish/completions/dev.fish");
        std::println("  pwsh:  dev completion pwsh | Invoke-Expression");
        return (argc < 2) ? 2 : 0;
    }

    auto plugins = discover_plugins();
    std::string_view shell = argv[1];

    if (shell == "bash")
        gen_bash(plugins);
    else if (shell == "zsh")
        gen_zsh(plugins);
    else if (shell == "fish")
        gen_fish(plugins);
    else if (shell == "pwsh")
        gen_pwsh(plugins);
    else {
        std::println(stderr, "completion: unknown shell '{}'", shell);
        std::println(stderr, "  supported: bash, zsh, fish, pwsh");
        return 1;
    }

    return 0;
}
