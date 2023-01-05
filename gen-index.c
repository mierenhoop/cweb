#define execute_index fputs("<!DOCTYPE html>\n" \
"<html>\n" \
"<head>\n" \
"<title>Val: ",stdout);printf("%s", value );fputs("</title>\n" \
"</head>\n" \
"<body>\n" \
"",stdout); if (some_number > 3) { fputs("\n" \
"<p>Number is bigger than 3</p>\n" \
"",stdout); } fputs("\n" \
"</body>\n" \
"</html>\n" \
"",stdout);
