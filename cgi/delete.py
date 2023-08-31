
import os
import sys

file_path = sys.argv[1:][0]

print("Content-type: text/html\n")

try:
    os.remove(file_path)
    success = "true"
except OSError as e:
    success = "false"

if success == "true":
	print("<html><body>")
	print(f"<h1>Resource Deleted</h1>")
	print(f"<p>Resource '{file_path}' has been deleted.</p>")
	print("</body></html>")
elif success == "false":
	print("<html><body>")
	print("<h1>Bad Request</h1>")
	print(f"<p>Resource '{file_path}' unavailable.</p>")
	print("</body></html>")