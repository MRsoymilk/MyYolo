from flask import Blueprint, render_template, send_from_directory
import os

make_sense_bp = Blueprint(
    'make_sense',
    __name__,
    static_folder="../../../vendor/make-sense-1.11.0-alpha/dist/assets",
    template_folder='../../../vendor/make-sense-1.11.0-alpha/dist'
)

@make_sense_bp.route('/make_sense')
def make_sense():
    return send_from_directory(
        os.path.join(os.path.dirname(__file__), '../../../vendor/make-sense-1.11.0-alpha/dist'),
        'index.html'
    )

@make_sense_bp.route("/assets/<path:path>")
def serve_static(path):
    return send_from_directory(
        os.path.join(os.path.dirname(__file__), '../../../vendor/make-sense-1.11.0-alpha/dist/assets'),
        path
    )

@make_sense_bp.route("/ico/<path:path>")
def serve_ico(path):
    return send_from_directory(
        os.path.join(os.path.dirname(__file__), '../../../vendor/make-sense-1.11.0-alpha/dist/ico'),
        path
    )

label_bp = Blueprint('label', __name__, template_folder='../../templates')

@label_bp.route('/label')
def label():
    return render_template('label.html')
