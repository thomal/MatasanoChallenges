echo "$1" | xxd -g0 | cut -d' ' -f1 --complement | cut -d' ' -f1 | tr --delete '\n'; echo ""
