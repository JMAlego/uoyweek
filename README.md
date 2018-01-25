# UoYWeek
UoYWeek gives the current date string formatted as used by the [University of York](https://www.york.ac.uk/). For example, `spr/3/wednesday` would refer to Wednesday on the 3rd week of Spring term.

_This is a fork, which probably won't be updated, which converted uoyweek from C++ to C._

## Installing
### Linux/Mac
```bash
# Clone the repository
git clone https://github.com/LukeMoll/uoyweek.git
cd uoyweek
# Compile
make uoyweek
chmod +x uoyweek
```
You now have an executable file, `uoyweek`. To run it while you're in `uoyweek/`, just type `./uoyweek`. You'll probably want it in a more useful place though.
```bash
sudo mv uoyweek /usr/local/bin
```
You should now be able to run `uoyweek` from any directory.

You can also pass it the `--fancy` argument to get proper capitalisation, for example `Spr/3/Wednesday`.

<!-- TODO Windows instructions -->
