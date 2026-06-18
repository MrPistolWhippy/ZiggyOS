#!/usr/bin/env python3
# ==============================================================================
#  ZIGGYOS SQLITE INTERCEPTOR ENGINE & JSON TELETEX WRAPPER
# ==============================================================================
import sqlite3, json, sys

def query_database_safe(query_str=None, params=None):
    try:
        # Establish direct loopback bridge to our active system data store
        conn = sqlite3.connect('/root/archive.db')
        cursor = conn.cursor()
        
        # Implement dynamic search filters if command arguments are supplied
        if query_str:
            cursor.execute(f"SELECT * FROM book_archive WHERE title LIKE ? OR author LIKE ?", (f"%{query_str}%", f"%{query_str}%"))
        else:
            cursor.execute("SELECT * FROM book_archive;")
            
        rows = cursor.fetchall()
        conn.close()
        
        formatted_data = []
        for x in rows:
            # Map raw array rows straight to system parameter tracking sheets
            item = {
                "asset_id": x[0],
                "title": x[1],
                "author": x[2],
                "publisher_or_name": x[3],
                "location": x[4],
                "notes": x[5],
                "observ_id": x[6] if len(x) > 6 else None
            }
            formatted_data.append(item)
            
        return json.dumps(formatted_data, indent=2)
        
    except Exception as e:
        return json.dumps([{"error": str(e)}], indent=2)

if __name__ == "__main__":
    search_term = sys.argv[1] if len(sys.argv) > 1 else None
    print(query_database_safe(search_term))
