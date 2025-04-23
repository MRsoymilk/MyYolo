from flask import Blueprint, render_template, request, jsonify
import subprocess
import threading

train_bp = Blueprint('train', __name__)

@train_bp.route('/train', methods=['GET'])
def train():
    return render_template('train.html')

@train_bp.route('/start_train', methods=['POST'])
def start_train():
    data = request.get_json()

    _weights = data.get('weights')
    _cfg = data.get('cfg')
    _hyp = data.get('hyp')
    _data = data.get('data')
    _project = data.get('project')
    _name = data.get('name')
    _epochs = data.get('epochs')
    _batch_size = data.get('batch-size')
    _img_size = data.get('img-size')

    if not _weights or not _cfg or not _hyp or not _data or not _project or not _name or not _epochs or not _batch_size or not _img_size:
        return jsonify({"error": "Missing parameters"}), 400

    thread = threading.Thread(target=run_training_script, args=(_weights, _cfg, _hyp, _data, _project, _name, _epochs, _batch_size, _img_size))
    thread.start()
    
    return jsonify({"message": "Training started..."})

def run_training_script(param1, param2):
    subprocess.run(['python', 'train.py', param1, param2])
