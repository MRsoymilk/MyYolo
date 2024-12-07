from flask import Flask
from app.blueprints import register_blueprints

def create_app():
    app = Flask(__name__)
    app.static_folder = '../static'

    register_blueprints(app)

    return app

