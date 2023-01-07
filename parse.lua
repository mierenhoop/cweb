local name = "index"

local sub = string.sub

local s = io.read"*a"

local write=io.write
local i = 1

local last = 1

local state = "html"

local function peek(n)
  return sub(s, i, i + (n or 1) - 1)
end

local function consume(n)
  i = i + (n or 1)
end

write("#define execute_"..name.." do { fputs(\"")
while i <= #s do
  if state ~= "html" and peek() == '"' then
    local match = peek()
    repeat
      consume()
      if peek() == "\\" then consume(2) end
    until peek() == match or i > #s
    consume()
  elseif peek(2) == "<%" then
    if state ~= "html" then error(parse_error) end
    -- strip newline if it precedes
    -- TODO: skip all spaces/tabs before too?
    write((sub(s, last, i-1):gsub("\"","\\\""):gsub("\n","\\n\" \\\n\"")))
    write("\",stdout);")
    consume(2)
    local formatstring
    while peek() ~= " " do
      formatstring = (formatstring or "%")..peek()
      consume()
    end
    if formatstring then
      state="inline"
      write("printf(\""..formatstring.."\",")
    else
      state="block"
    end
    last=i
  elseif peek(2) == "%>" then
    if state == "html" then return error(parse_error) end
    write(sub(s, last, i-1))
    if state == "inline" then
      write(");")
    end
    write("fputs(\"")
    consume(2)
    state = "html"
    last=i
  else
    consume()
  end
end
write((sub(s, last, i-1):gsub("\"","\\\""):gsub("\n","\\n\" \\\n\"")))
write("\",stdout); } while (0)\n")
