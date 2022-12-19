### Prerequisites, macOS on Apple Silicon (arm64)
AFAIK, GCC 6 isn't available on the Apple Silicon / arm64 architecture, so we need to do this bit using the Rosetta2 x86 translation layer.

1. Open Finder, go to /Applications/Utilities
2. Right-click the Terminal app and select "Duplicate"
3. Rename the duplicate to something like "TerminalIntel" to easier keep track of the different versions
4. Right-click the duplicate app and select "Get Info"
5. Put a checkmark in the "Open using Rosetta" box
6. Double-click the app to open it (using Rosetta)
7. Verify that you're using the i386 arch by typing `arch`
8. Install [Homebrew](https://brew.sh) on the i386 arch:
```
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
Homebrew will be installed as `/usr/local/bin/brew`.

9. Modify your `~/.zshrc` to make the `brew` command point to the i386 homebrew binary by adding these lines:
```
if [ `arch` = 'i386' ]
then
  alias brew="/usr/local/bin/brew"
fi
```
10. Reload the file: `$ source ~/.zshrc`

11. Install gcc version 6:
```
$ brew install gcc@6
$ ln -s /usr/local/bin/gcc-6 /usr/local/bin/gcc
$ ln -s /usr/local/bin/g++-6 /usr/local/bin/g++
```
12. Install gnu-tar and wget:
```
$ brew install gnu-tar wget
```