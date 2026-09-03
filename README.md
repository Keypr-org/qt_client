# Keypr — Desktop Client

> Repository containing the code of the desktop application.

Keypr is a cross-platform password manager built with Qt6/C++. This repository holds the desktop client (UI, controllers, forms) and pulls in [`keypr-core`](https://github.com/Keypr-org/core) as a git submodule for vault encryption, decryption and parsing. Vaults are encrypted locally with `libsodium`; nothing about your secrets is sent anywhere unless you explicitly enable a feature (e.g. email aliases) that requires it.

## Table of Contents

- [Keypr — Desktop Client](#keypr--desktop-client)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Project Structure](#project-structure)
  - [How to Use](#how-to-use)
    - [Creating and unlocking a vault](#creating-and-unlocking-a-vault)
    - [Managing entries](#managing-entries)
    - [Personas](#personas)
    - [Password generator](#password-generator)
    - [Using email aliases](#using-email-aliases)
  - [How to Build from Source](#how-to-build-from-source)
    - [First time cloning the repository](#first-time-cloning-the-repository)
    - [Updating the repository and its submodules](#updating-the-repository-and-its-submodules)
    - [Automatically updating the submodules when pulling](#automatically-updating-the-submodules-when-pulling)
    - [Updating the submodules in the repository](#updating-the-submodules-in-the-repository)
    - [Dependencies](#dependencies)
    - [Build steps](#build-steps)
  - [Native Messaging](#native-messaging)
  - [Contributions and Workflow](#contributions-and-workflow)
    - [Contributing](#contributing)
    - [Workflow of the project](#workflow-of-the-project)
      - [Example of a versioned tag: `v1.0.2`](#example-of-a-versioned-tag-v102)
  - [AI Usage](#ai-usage)

## Features

- **Encrypted vaults** — create one or several vaults, each unlocked with its own master password (`keypr-core` handles the crypto, backed by `libsodium`).
- **Categorized entries** — organize secrets as Website, Credit Card and WiFi entries inside categories.
- **Personas** — generate and reuse fake identities (name, address, etc.) when filling out web entries.
- **Password generator** — generate strong passwords with a live password-strength indicator.
- **Email aliases** — generate disposable email aliases for Website entries through the [Postscale](https://postscale.io) service.
- **Cross-platform** — builds and ships for Windows, macOS and Linux via the project's CI/CD pipeline.

| | |
|---|---|
| **Vault selection**<br>![Vault selection](resources/screenshots/keypr_mainpage_vault_selection.png) | **Vault unlock**<br>![Vault unlock](resources/screenshots/keypr_vault_unlock_incorrect_password.png) |
| **Category list**<br>![Category list](resources/screenshots/keypr_category_list_vault_unlocked.png) | **Create a vault**<br>![Create vault](resources/screenshots/keypr_vault_creation_overlay.png) |
| **Create an entry**<br>![Create entry overlay](resources/screenshots/keypr_create_entry_overlay.png) | **Website entry**<br>![Website entry](resources/screenshots/keypr_web_entry.png) |
| **Create website entry**<br>![Create website entry](resources/screenshots/keypr_create_web_entry.png) | **Card entry**<br>![Card entry](resources/screenshots/keypr_card_entry.png) |
| **Create card entry**<br>![Create card entry](resources/screenshots/keypr_create_card_entry.png) | **WiFi entry**<br>![WiFi entry](resources/screenshots/keypr_wifi_entry.png) |
| **Create WiFi entry**<br>![Create WiFi entry](resources/screenshots/keypr_create_wifi_entry.png) | **Password generator**<br>![Password generator](resources/screenshots/keypr_generate_password_auto.png) |
| **Personas**<br>![Personas](resources/screenshots/keypr_personas_display.png) | **Create persona**<br>![Create persona](resources/screenshots/keypr_persona_creation.png) |

## Project Structure

```
qt_client/
├── CMakeLists.txt          # Top-level build definition (qt_client target)
├── CMakePresets.json        # CMake presets (toolchain, generator, ...)
├── vcpkg.json               # Client dependencies (gtest, libsodium, nlohmann-json)
├── resources.qrc            # Qt resource file (icons, images)
├── icons/                   # Icon assets used throughout the UI
├── resources/
│   ├── logo.png
│   └── screenshots/          # Screenshots used in this README
├── keypr-core/               # Git submodule: vault encryption/decryption/parsing library
│   ├── include/               # Public headers of keypr-core
│   ├── src/                   # Implementation of keypr-core
│   └── tests/                 # keypr-core unit tests
├── src/
│   ├── main.cpp                 # Application entry point
│   ├── mainwindow.*              # Main window shell
│   ├── vaultcontroller.*         # Vault lifecycle (create/unlock/lock, entries CRUD)
│   ├── mailaliasclient.*         # HTTP client for the Postscale API
│   ├── mailaliascontroller.*     # Business logic wiring the alias client to the UI
│   ├── appconfig.* / jsonfileconfig.*   # Application configuration (JSON-backed)
│   ├── vaultstoragesetupdialog.* # First-run vault storage location setup
│   ├── component/                # Reusable UI widgets (inputs, cards, list items, tooltips, toggle switch...)
│   ├── formOverlay/               # Modal overlays (create vault, create category, new entry, edit persona)
│   ├── mainContent/                # Main content area screens
│   │   └── entries/                  # Per-entry-type widgets (website, credit card, wifi)
│   ├── sideBar/                    # Sidebar widgets (vault selection, category selection)
│   ├── settings/                   # Settings window
│   └── utils/                      # Small helpers (card number formatting, password strength, random persona, hover/clickable widgets)
├── tests/                    # Qt client unit tests (vault controller, mail alias controller)
└── .github/workflows/        # CI/CD: build-and-test.yml, package-for-release.yml
```

## How to Use

### Creating and unlocking a vault

1. On first launch, choose where vault files should be stored.
2. Create a new vault and set its master password.
3. Select the vault and unlock it with its master password to access its entries.

### Managing entries

Inside an unlocked vault, create categories and add entries of type **Website**, **Credit Card** or **WiFi**. Entries can be edited, copied to clipboard and deleted from the category view.

### Personas

Use the Personas panel to generate reusable fake identities, which can then be picked when filling out a Website entry's form fields.

### Password generator

Open the password generator from a form to create a strong password; the generator shows a live strength indicator before you apply it to the field.

### Using email aliases

For email aliases, we are using the [Postscale](https://postscale.io) service. To generate email aliases through the application, here are the steps to follow:
1. Register to Postscale
2. Follow the `Getting Started` instruction until the `adding a domain` step
3. Get your API key and target email your wish emails to be redirected to
4. Paste those informations inside the `Settings` window inside Keypr
5. Click generate alias inside a WEB entry and enjoy !

## How to Build from Source

### First time cloning the repository

```bash
# HTTPS
git clone --recurse-submodules https://github.com/Keypr-org/core.git
# SSH
git clone --recurse-submodules git@github.com:Keypr-org/core.git
```

### Updating the repository and its submodules

```bash
git pull --recurse-submodules
git submodule update --init --recursive
```

### Automatically updating the submodules when pulling

You can alternatively configure git to automatically update the submodules when pulling:

```bash
git config --global submodule.recurse true
```

### Updating the submodules in the repository

```bash
git submodule update --remote --merge
git add keypr-core # Assuming the submodule is located in the keypr-core folder
git commit -m "[Fix]: Update submodule keypr-core"
git push
```

### Dependencies

- CMake >= 3.16
- A C++20 compiler
- Qt6 >= 6.5 (Widgets, Test, Network components)
- [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash), used to fetch `gtest`, `libsodium` and `nlohmann-json` (for both `qt_client` and the `keypr-core` submodule)

### Build steps

Configure the project, pointing CMake at your vcpkg toolchain file:

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Build it:

```bash
# Build the debug version
cmake --build --workflow default

# Build the release version
cmake --build --workflow release
```

You can also direclty build the project inside QT Creator, which is the easiest option, by opening the `CMakeLists.txt` file and **configuring the project with the vcpkg toolchain file**. You can do so by going to `Projects` > `Build & Run`, select your build kit > `CMake` > `CMake Toolchain File` and select the vcpkg toolchain file. Then you can build the project by clicking on the `Build` button.

```

### Running the tests

Tests are built with the project (Qt Test + GoogleTest for `keypr-core`). Run them via `ctest` from the build directory:

```bash
ctest --test-dir build
```

## Native Messaging

The application supports [Native Messaging](https://developer.chrome.com/docs/apps/nativeMessaging/) for browser extensions. The application behaves as a native messaging host, allowing extensions to communicate with it. The web extension will open an instance of the application if it is not already running, and send messages to it. The application will respond with the requested data, such as credentials for a specific website (email + password).

## Contributions and Workflow

### Contributing

Contributions are welcome! Please read our contribution guidelines before submitting a pull request.

### Workflow of the project

For the development of this specific repository, we will use [this](.github/workflows/package-for-release.yml) workflow.

**What it does :**

- On push or on a pull request to the `main` branch, it will build the project and run the tests. If every steps are successful, it will create artifacts available for download. Those artifacts are executables for Windows, MacOS and Linux. The artifacts are zipped and available for download in the `Latest build` realease of the repository (tagged as Pre-release).
- On a tagged push/pull request (verisoned tag starting like v.* p.ex: v1.0.2), it will build the project and run the tests. If every steps are successful, it will create artifacts available for download. The workflow will then create a release with the versioned tag and attach the artifacts to the release. The release will be available in the `Releases` section of the repository tagged as `Latest`.

#### Example of a versioned tag: `v1.0.2`

```bash
# This will create a tag v1.0.2 and push it to the remote repository creating a release with the tag v1.0.2 and attaching the artifacts to the release.
git tag v1.0.2 && git push origin --tags
```
---

## AI Usage

Parts of this project's documentation and/or code have been assisted by AI tools. AI-assisted contributions are carefully reviewed by us before being merged, same as any other contribution. We've used AI to mainly help for reasearch, PR summaries and documentation.
