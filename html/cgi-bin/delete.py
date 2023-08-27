
import os
import sys

file_path = sys.argv[1:][0]

success = sys.argv[1:][1]

print("Content-type: text/html\n")

if success == "true":
	print("<html><body>")
	print(f"<h1>Resource Deleted</h1>")
	print(f"<p>Resource '{file_path}' has been deleted.</p>")
	print("</body></html>")
else:
	print("<html><body>")
	print("<h1>Bad Request</h1>")
	print(f"<p>Resource '{file_path}' unavailable.</p>")
	print("</body></html>")