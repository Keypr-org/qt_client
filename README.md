# qt_client

> Repository containing the code of the desktop application.

## Contributions and Workflow

### Contributing

Contributions are welcome! Please read our contribution guidelines before submitting a pull request.

### Workflow of the project

For the development of this specific repository, we will use [this](.github/workflows/qt_client.yml) workflow.

**What it does :**

- On push or on a pull request to the `main` branch, it will build the project and run the tests. If every steps are successful, it will create artifacts available for download. Those artifacts are executables for Windows, MacOS and Linux. The artifacts are zipped and available for download in the `Latest build` realease of the repository (tagged as Pre-release).
- On a tagged push/pull request (verisoned tag starting like v.* p.ex: v1.0.2), it will build the project and run the tests. If every steps are successful, it will create artifacts available for download. The workflow will then create a release with the versioned tag and attach the artifacts to the release. The release will be available in the `Releases` section of the repository tagged as `Latest`.

#### Example of a versioned tag: `v1.0.2`

```bash
# This will create a tag v1.0.2 and push it to the remote repository creating a release with the tag v1.0.2 and attaching the artifacts to the release.
git tag v1.0.2 && git push origin --tags
```
