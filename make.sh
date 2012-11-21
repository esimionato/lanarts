mkdir -p ../lanarts_build
cd ../lanarts_build

CXX=/usr/bin/clang++ CC=/usr/bin/clang cmake ../lanarts

## START TIMER CODE
T="$(date +%s%N)"
## START TIMER CODE

##MAKE
make -j4

## END TIMER CODE
T="$(($(date +%s%N)-T))"
echo "Total build time: $((T/1000000))ms"
## END TIMER CODE
