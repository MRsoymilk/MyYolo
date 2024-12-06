from flask import Flask, render_template, Blueprint, send_from_directory
import os

app = Flask(__name__)
app.static_folder = 'static'
app.add_url_rule('/lib/<path:filename>', endpoint='lib', view_func=lambda filename: app.send_static_file(f'lib/{filename}'))


make_sense_bp = Blueprint('make_sense', 
                            __name__,
                            static_folder="../vendor/make-sense-1.11.0-alpha/dist/assets",
                            template_folder='../vendor/make-sense-1.11.0-alpha/dist'
                            )

@make_sense_bp.route('/make_sense')
def make_sense():
    #return render_template('index.html')
    return send_from_directory(os.path.join(app.root_path, '../vendor/make-sense-1.11.0-alpha/dist/'), 'index.html')


@make_sense_bp.route("/assets/<path:path>")
def serve_static(path):
    return send_from_directory(os.path.join(app.root_path, '../vendor/make-sense-1.11.0-alpha/dist/assets'), path)

@make_sense_bp.route("/ico/<path:path>")
def serve_ico(path):
    return send_from_directory(os.path.join(app.root_path, '../vendor/make-sense-1.11.0-alpha/dist/ico'), path)

app.register_blueprint(make_sense_bp)

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/add_data')
def add_data():
    return render_template('add_data.html')

@app.route('/train')
def train():
    return render_template('train.html')

@app.route('/filter')
def filter_data():
    return render_template('filter.html')

@app.route('/test')
def test():
    return render_template('test.html')

@app.route('/export')
def export():
    return render_template('export.html')

if __name__ == '__main__':
    app.run(debug=True)

