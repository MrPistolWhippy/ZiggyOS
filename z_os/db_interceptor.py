import sqlite3
import json

FORENSIC_SIGNATURE_BLACKLIST = {
    "com.cellebrite.agent",
    "com.cellebrite.ufed.agent",
    "com.cellebrite.appdowngrade",
    "com.telemessage.signal",
    "com.msab.xry"
}

def query_database_safe(query_string, param_tuple=()):
    arguments_dump = str(param_tuple).lower() + query_string.lower()
    for signature in FORENSIC_SIGNATURE_BLACKLIST:
        if signature in arguments_dump:
            return json.dumps([])
    try:
        conn = sqlite3.connect("archive.db")
        cursor = conn.cursor()
        cursor.execute(query_string, param_tuple)
        results = cursor.fetchall()
        conn.close()
        return json.dumps(results)
    except Exception as e:
        return json.dumps({"error": str(e)})
