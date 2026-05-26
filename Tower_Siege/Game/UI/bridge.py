"""
Bridge module for Tower Siege UI.
Handles JSON file I/O and engine execution.
"""
import json
import os
import subprocess

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
INPUT_PATH = os.path.join(ROOT_DIR, "data", "input.json")
STATE_PATH = os.path.join(ROOT_DIR, "data", "state.json")
ENGINE_PATH = os.path.join(ROOT_DIR, "Engine", "tower_siege_engine.exe")


def load_json(path):
    """Carga y devuelve el JSON contenido en `path`.

    Lanza excepción si el archivo no existe o no es legible.
    """
    with open(path, "r", encoding="utf-8") as file:
        return json.load(file)


def save_json(path, data):
    """Serializa `data` como JSON y lo escribe en `path`.

    Sobrescribe el archivo si ya existía.
    """
    with open(path, "w", encoding="utf-8") as file:
        json.dump(data, file, indent=2)


def load_input():
    """Devuelve el contenido de `input.json` como estructura Python."""
    return load_json(INPUT_PATH)


def save_input(data):
    """Escribe `data` en `input.json`."""
    save_json(INPUT_PATH, data)


def load_state():
    """Devuelve el contenido de `state.json` como estructura Python."""
    return load_json(STATE_PATH)


def save_state(data):
    """Escribe `data` en `state.json`."""
    save_json(STATE_PATH, data)


def run_engine():
    """Ejecuta el binario del motor si existe.

    Retorna True si la ejecución finaliza con código 0, False en caso
    contrario (o si no existe el ejecutable).
    """
    if not os.path.exists(ENGINE_PATH):
        return False
    try:
        subprocess.run([ENGINE_PATH], cwd=ROOT_DIR, check=True)
        return True
    except subprocess.CalledProcessError:
        return False
    except OSError:
        return False
