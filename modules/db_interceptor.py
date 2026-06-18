import sqlite3
import json

def query_database_safe(query_str=None, params=None):
    try:
        conn = sqlite3.connect('archive.db')
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM book_archive;")
        rows = cursor.fetchall()
        conn.close()
        
        formatted_data = []
        for r in rows:
            formatted_data.append([
                r[0], # asset_id
                r[1], # title
                r[2], # author
                r[3], # publisher_or_name
                r[4], # location
                r[5], # notes
                r[6], # reserve_nzd
                str(r[7]) if len(r) > 7 and r[7] is not None else "150.0" # value
            ])
        # Wrap the list array inside a clean JSON text string structure
        return json.dumps(formatted_data)
    except Exception as e:
        return json.dumps([])
