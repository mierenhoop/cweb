
#define execute_index do { \
    fputs("<!DOCTYPE html>\n" \
          "<html>\n" \
          "<title>Val: ", stdout); printf("%s",value); fputs("</title>\n" \
          "</head>\n" \
          "<body>\n", stdout); \
          if (some_number > 3) { \
    fputs("<p>Number is bigger than 3</p>\n", stdout); \
          } \
      fputs("</body>\n" \
           "</html>", stdout); } while(0)
