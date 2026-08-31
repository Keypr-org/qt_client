# qt_client 

> Repository containing the code of the desktop application.

## Table of Contents

- [qt\_client](#qt_client)
  - [Table of Contents](#table-of-contents)
  - [Cloning/Updating the repository and its submodules](#cloningupdating-the-repository-and-its-submodules)
    - [First time cloning the repository](#first-time-cloning-the-repository)
    - [Updating the repository and its submodules](#updating-the-repository-and-its-submodules)
    - [Automatically updating the submodules when pulling](#automatically-updating-the-submodules-when-pulling)
  - [Updating the submodules in the repository](#updating-the-submodules-in-the-repository)
  - [How to use the application](#how-to-use-the-application)
    - [Using email aliases](#using-email-aliases)
  - [Contributions and Workflow](#contributions-and-workflow)
    - [Contributing](#contributing)
    - [Workflow of the project](#workflow-of-the-project)
      - [Example of a versioned tag: `v1.0.2`](#example-of-a-versioned-tag-v102)

## Cloning/Updating the repository and its submodules

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
## Updating the submodules in the repository

```bash
git submodule update --remote --merge
git add keypr-core # Assuming the submodule is located in the keypr-core folder
git commit -m "[Fix]: Update submodule keypr-core" 
git push
```

## How to use the application

### Using email aliases

For email aliases, we are using the [Postscale](https://postscale.io) service. To generate email aliases through the application, here are the steps to follow :
1. Register to Postscale
2. Follow the `Getting Started` instruction until the `adding a domain` step
3. Get your API key and target email your wish emails to be redirected to
4. Paste those informations inside the `Settings` window inside Keypr
5. Click generate alias inside a WEB entry and enjoy !

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
