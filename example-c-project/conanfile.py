from conans import ConanFile, Meson


class ExamplecprojectConan(ConanFile):
    name = "example-c-project"
    version = "0.0.1"
    author = "Jakub Pieńkowski"
    url = "https://github.com/Jakski/"
    description = "Example C project"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "pkg_config"
    exports_sources = "src/*"
    requires = "jansson/2.12"

    def build(self):
        meson = Meson(self)
        defs = {}
        if self.settings.build_type == "Debug":
            defs["b_sanitize"] = "address"
        meson.configure(defs=defs, source_folder="src")
        meson.build()
        self.run("ninja test", run_environment=True)

    def configure(self):
        del self.settings.compiler.libcxx

    def package(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy(
            "example-c-project",
            src="bin", dst="bin", keep_path=False)
