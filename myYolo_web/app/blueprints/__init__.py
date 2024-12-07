from flask import Blueprint
from app.blueprints.root import root_bp
from app.blueprints.export import export_bp
from app.blueprints.train import train_bp
from app.blueprints.add_data import add_data_bp
from app.blueprints.label import label_bp, make_sense_bp
from app.blueprints.filter import filter_bp
from app.blueprints.test import test_bp

def register_blueprints(app):
    app.register_blueprint(root_bp)
    app.register_blueprint(export_bp)
    app.register_blueprint(train_bp)
    app.register_blueprint(add_data_bp)
    app.register_blueprint(label_bp)
    app.register_blueprint(make_sense_bp)
    app.register_blueprint(filter_bp)
    app.register_blueprint(test_bp)

