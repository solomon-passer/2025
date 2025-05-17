def rate(origin, userInput):
    '''使用生成器表达式'''
    if not (isinstance(origin, str) and isinstance(userInput, str)):
        print('The two parameters must be strings.')
        return
    if len(origin) == 0:
        print('The length of origin string cannot be zero.')
        return
    if len(origin) < len(userInput):
        print('Sorry. I suppose the second parameter string is shorter.')
        return
    right = sum((1 for oc, uc in zip(origin, userInput) if oc==uc))
    return right / len(origin)

origin = 'lkehefkjdngldjhfdds'
userInput = input("Please enter a string: ")
result = rate(origin, userInput)
if result is not None:
    print(result)
