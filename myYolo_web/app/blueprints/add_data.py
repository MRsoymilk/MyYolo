from flask import Blueprint, render_template, send_from_directory
import os

add_data_bp = Blueprint('add_data', __name__, template_folder='../../templates')

@add_data_bp.route('/add_data')
def add_data():
    return render_template('add_data.html')
