import os


def list_directory(dir_name):
    '''
    List the files contained into the directory.
    Returns:
    file_list - A list containing all files in dir_name.
    '''

    file_list = []
    for root, dirs, files in os.walk(dir_name):
        for file in files:
            root = root.rstrip("/")
            file_list.append(root+"/"+file)
    return file_list
