{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    qmk_firmware = {
      url = "https://github.com/qmk/qmk_firmware.git";
      flake = false;
      type = "git";
      submodules = true;
    };
    qmkfmt = {
      url = "github:rcorre/qmkfmt?ref=v0.2.0";
      flake = false;
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      qmkfmt,
      qmk_firmware,
    }:
    let
      keyboards = [
        {
          keyboard = "hotdox76v2";
          name = "hotdox76";
          rules = ./keyboards/hotdox76/rules.mk;
          config = ./keyboards/hotdox76/config.h;
          keymap = ./keyboards/hotdox76/keymap.c;
        }
      ];
      pkgs = import nixpkgs {
        system = "x86_64-linux";
      };
      qmkfmtPkg = pkgs.rustPlatform.buildRustPackage {
        pname = "qmkfmt";
        version = "v0.2.0";
        src = qmkfmt;
        cargoHash = "sha256-jSrGrYCJxqgp7GvcWZPGriJ5hw+Qfm9K/Po3Ay6WyzI=";
        doCheck = false;
      };
      flash = pkgs.writeShellScriptBin "flash" ''
        set -e

        FIRMWARE_PATH="$1"
        if [ -z "$FIRMWARE_PATH" ]; then
          echo "Usage: flash <path_to_firmware.hex>"
          exit 1
        fi

        ${pkgs.qmk}/bin/qmk flash "$FIRMWARE_PATH"
      '';

      firmware = pkgs.stdenv.mkDerivation {
        pname = "qmk_firmware";
        version = "v1.0";
        src = qmk_firmware;
        nativeBuildInputs = [
          pkgs.gnumake
          pkgs.qmk
        ];
        patchPhase = builtins.concatStringsSep "\n" (
          map (k: ''
            cp ${k.rules} keyboards/${k.keyboard}/rules.mk
            cp ${k.config} keyboards/${k.keyboard}/config.h

            mkdir -p keyboards/${k.keyboard}/keymaps/${k.name}
            cp ${k.keymap} keyboards/${k.keyboard}/keymaps/${k.name}/keymap.c
          '') keyboards
        );
        buildPhase = builtins.concatStringsSep "\n" (
          map (k: ''
            SKIP_GIT=true make -r -R -f builddefs/build_keyboard.mk -s KEYBOARD=${k.keyboard} KEYMAP=${k.name} KEYBOARD_FILESAFE=${k.keyboard} TARGET=${k.name} VERBOSE=false COLOR=true SILENT=false
          '') keyboards
        );
        installPhase = ''
          mkdir -p $out/firmware
          mkdir -p $out/bin
          cp -r .build/*.hex $out/firmware/
          cp ${flash}/bin/flash $out/bin
        '';
      };
    in
    {

      packages.x86_64-linux.firmware = firmware;
      packages.x86_64-linux.default = firmware;

      devShells.x86_64-linux.default = nixpkgs.legacyPackages.x86_64-linux.mkShell {
        buildInputs = [
          qmkfmtPkg
          pkgs.qmk
          pkgs.ccls
          pkgs.clang-tools
        ];
      };

    };
}
