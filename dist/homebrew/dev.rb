class Dev < Formula
  desc "Lightweight CLI dispatcher for developers"
  homepage "https://github.com/zuudevs/dev"
  url "https://github.com/zuudevs/dev/archive/refs/tags/v1.0.0.tar.gz"
  sha256 ""  # Fill after release
  license "MIT"

  depends_on "cmake" => :build

  def install
    system "cmake", "-B", "build",
           "-DCMAKE_BUILD_TYPE=Release",
           "-DCMAKE_INSTALL_PREFIX=#{prefix}",
           *std_cmake_args
    system "cmake", "--build", "build", "--config", "Release"

    bin.install "build/bin/Release/Dev" => "dev"

    # Install plugins
    (libexec/"plugins").install Dir["plugins/*"]

    # Wrapper script that sets plugin path
    (bin/"dev").write <<~EOS
      #!/bin/bash
      exec "#{libexec}/dev" "$@"
    EOS
  end

  test do
    assert_match "dev v#{version}", shell_output("#{bin}/dev --version")
    assert_match "hello", shell_output("#{bin}/dev list")
  end
end
