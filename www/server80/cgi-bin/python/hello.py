import string
import glob
import os
import json
import datetime

currentdir = os.path.dirname(__file__)
templatedir = currentdir + '/template'
jsondir = currentdir + '/json'
dt_now = datetime.datetime.now()
# print(currentdir)
# print(templatedir)
# print(jsondir)

# read html templete file
text = ''
with open(templatedir + '/index.html', 'r', encoding='utf-8') as f:
    text = f.read()

# load json file as json object, add time
with open( jsondir + '/text.json', 'r') as f:
    j = json.load(f)
    j['time'] = dt_now
    j['title'] = "Page made from Python program"
    j['message'] = "This page is made by Python, templete html and data json. <br>You can change this message by changing hello.py file, or json file"


# make a template from text, and replace the text with json data
result = ''
template_text = string.Template(text)
result = template_text.safe_substitute(j)

print(result)