from flask import Blueprint, render_template

filter_bp = Blueprint('filter', __name__)

@filter_bp.route('/filter')
def filter_data():
    return render_template('filter.html')

