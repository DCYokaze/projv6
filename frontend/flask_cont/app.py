import os
#import magic
import urllib.request
#from app import app
from flask import Flask, flash, request, redirect, render_template
from werkzeug.utils import secure_filename
import subprocess

UPLOAD_FOLDER = './upHere'
ALLOWED_EXTENSIONS = set(['txt', 'pdf', 'png', 'jpg', 'jpeg', 'gif'])

app = Flask(__name__)
app.secret_key = "secret key"
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config['MAX_CONTENT_LENGTH'] = 4 * 1024 * 1024


def allowed_file(filename):
	return True  # get all file for now.
	#return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/')
def upload_form():
	return render_template('upload.html')

@app.route('/', methods=['POST'])
def upload_file():
	if request.method == 'POST':
    # check if the post request has the file part
		if 'file' not in request.files:
			flash('No file part')
			return redirect(request.url)
		file = request.files['file']
		if file.filename == '':
			flash('No file selected for uploading')
			return redirect(request.url)
		if file and allowed_file(file.filename):
			# filename = secure_filename(file.filename)
			#file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
			file.save(os.path.join(app.config['UPLOAD_FOLDER'], "save.exe"))
			flash('File successfully uploaded')
			#st1 = os.system("ls upHere")
			
			#result = subprocess.getoutput(['ls', '-l', 'upHere'])#, stdout=subprocess.PIPE)
			#result = subprocess.getoutput("ls upHere")
			result = subprocess.getoutput("objdump -d upHere/save.exe")
			subprocess.getoutput("rm upHere/save.exe")
			#result.stdout
			res2 = result[0:1000]
			#print(result)
			flash(res2)
			#flash(result.stdout)
			return redirect('/')
		else:
			flash('Allowed file types are txt, pdf, png, jpg, jpeg, gif')
			return redirect(request.url)

if __name__ == "__main__":
    # app.run()
		app.run(debug=True,host='0.0.0.0',port=int(os.environ.get('PORT', 8080)))
