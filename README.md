# CollAuth

My own authentication server for educational purposes.

## Overview

CollAuth is an experimental OAuth 2.0 authorization server. It is being developed
primarily for educational purposes for me to explore low-level details as well as
security and cryptography using C++.

## Features

- **OAuth 2.0**: Aims to implement the OAuth 2.0 specification, with potential
  future support for newer versions as well as OpenID Connect (OIDC).
- **No Third-Party Dependencies**: Built from scratch using only standard C++23
  features for a deeper leaning experience.

## Target Audience

This project is intended for developers and students interested in learning about
OAuth 2.0 and server programming or C++ in general.

**Note**: This project is currently for educational purposes only and not production-ready.

## Getting Started

### Prerequisites

- A C++23-compatible compiler
- CMake for building the project

### Installation

1. Clone the repository:

   ```bash
   git clone git@gitlab.com:colllijo/collauth.git
   cd collauth
   ```

2. Build the project:

   ```bash
   cmake -S . -B build
   cmake --build build
   ```

3. Run the server:

   ```bash
   ./build/CollAuth
   ```

## Development

This project is developed by me in my free time. Contributions are welcome, especially
improvements and corrections to my implementations. But please not that this is
a learning project, and that I aim to be involved in the development of most features.

## License

This project is licensed under the AGPLv3. See the [LICENSE](LICENSE) file for details.
