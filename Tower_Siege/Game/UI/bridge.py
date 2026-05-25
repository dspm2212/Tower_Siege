# Team 12
# Variant: Tower Siege
# Students: Ana Pérez, Luis Gómez

import json
import os
import subprocess

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
INPUT_PATH = os.path.join(ROOT_DIR, "data", "input.json")
STATE_PATH = os.path.join(ROOT_DIR, "data", "state.json")
ENGINE_PATH = os.path.join(ROOT_DIR, "Engine", "tower_siege_engine.exe")


def load_json(path):
    with open(path, "r", encoding="utf-8") as file:
        return json.load(file)


def save_json(path, data):
    with open(path, "w", encoding="utf-8") as file:
        json.dump(data, file, indent=2)


def load_input():
    return load_json(INPUT_PATH)


def save_input(data):
    save_json(INPUT_PATH, data)


def load_state():
    return load_json(STATE_PATH)


def save_state(data):
    save_json(STATE_PATH, data)


def run_engine():
    if not os.path.exists(ENGINE_PATH):
        return False
    try:
        subprocess.run([ENGINE_PATH], cwd=os.path.dirname(ROOT_DIR), check=True)
        return True
    except subprocess.CalledProcessError:
        return False
    except OSError:
        return False
