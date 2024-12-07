from flask import Blueprint, render_template

export_bp = Blueprint('export', __name__)

@export_bp.route('/export')
def export():
    return render_template('export.html')

