from typing import Optional

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, cmake_layout


class cpp_project_template(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def _get_march_value(self) -> Optional[str]:
        """Extract march value from profile CXXFLAGS"""
        cxxflags: list[str] = self.conf.get(
            "tools.build:cxxflags", check_type=list, default=list()
        )

        for flag in cxxflags:
            if flag.startswith("-march="):
                return flag.split("=", 1)[1]
        return None

    def generate(self):
        tc = CMakeToolchain(self)

        cache_vars = {}

        preproc_defs = {
            "FFI_STATIC_BUILD": "1",
        }

        for var, val in cache_vars.items():
            tc.cache_variables[var] = val
        for var, val in preproc_defs.items():
            tc.preprocessor_definitions[var] = val

        tc.generate()

    def layout(self):
        build_layout = [
            "settings.compiler",
            "settings.compiler.version",
            "settings.build_type",
        ]
        march = self._get_march_value()
        if march is not None:
            build_layout.append(f"const.{march}")
        self.folders.build_folder_vars = build_layout
        cmake_layout(self)

    def requirements(self):
        reqs = {
            "fmt": "11.1.3",
            "frozen": "1.2.0",
            "boost": "1.87.0",
            "spdlog": "1.15.1",
        }  # defaults

        match self.settings.get_safe("os", None):
            case "Linux":
                pass

            case "Windows":
                pass

            case _:
                pass

        for name, version in reqs.items():
            self.requires(f"{name}/{version}")  # type: ignore
