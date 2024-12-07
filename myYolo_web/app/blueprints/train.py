from flask import Blueprint, render_template

train_bp = Blueprint('train', __name__)

@train_bp.route('/train')
def train():
    return render_template('train.html')

