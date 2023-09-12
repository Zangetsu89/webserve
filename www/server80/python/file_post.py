#!/usr/bin/env python3

# Still experimenting... Now this code just show the POST data.
# I'm trying to split the POST data and get the body data (not succeed yet), and it will be saved later

import sys
import select
import os
import subprocess
import re

data = sys.stdin.read()
# print(data)

# print(type(os.environ))
# print(os.environ)
cl = os.environ.get('CONTENT_LENGTH', 'none')
ct = os.environ.get('CONTENT_TYPE', 'none')
qs = os.environ.get('QUERY_STRING', 'none')
# print(cl)
# print(ct)
# print(qs)

match = re.search(r'boundary=(.*)', ct)

if match:
    boundary = match.group(1)
    # print(boundary)
else:
    print("no boundary")

if boundary:

    match = re.search(rf'{boundary}(.*?){boundary}', data, re.DOTALL)

    if match:
        extracted_data = match.group(1)
        print(extracted_data)
    else:
        print("no body")


success = "true"

if success == "true":
    print("Content-type: text/html\n")
    print("<html><body><h1>Data saved successfully.</h1></body></html>")
else:
    print("Content-type: text/html\n")
    print("<html><body><h1>Data cannot be saved.</h1></body></html>")

