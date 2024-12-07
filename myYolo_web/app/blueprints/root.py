from flask import Blueprint, render_template

root_bp = Blueprint('root', __name__, template_folder='../../templates')

@root_bp.route('/')
def home():
    return render_template('index.html')
