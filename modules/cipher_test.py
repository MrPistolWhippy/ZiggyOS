import sys

def calculate_ciphers(text):
    clean_text = "".join(c.upper() for c in text if c.isalpha())
    
    ordinal_sum = 0
    reduction_sum = 0
    kay_sum = 0
    
    for char in clean_text:
        val = ord(char) - ord('A') + 1
        ordinal_sum += val
        
        red_val = val if val < 10 else (val % 9 if val % 9 != 0 else 9)
        reduction_sum += red_val
        
        if char >= 'K':
            kay_sum += (val + 26)
        else:
            kay_sum += val
            
    return ordinal_sum, reduction_sum, kay_sum

if __name__ == "__main__":
    test_word = "ZIGGY"
    if len(sys.argv) > 1:
        test_word = sys.argv[1]
        
    ord_s, red_s, kay_s = calculate_ciphers(test_word)
    
    print("==================================================")
    print(f"ALPHANUMERIC CIPHER ENGINE TARGET: {test_word}")
    print("==================================================")
    print(f" -> English Ordinal Value   : {ord_s}")
    print(f" -> English Reduction Value : {red_s}")
    print(f" -> Kay Fraternal Value     : {kay_s}")
    print("==================================================")
