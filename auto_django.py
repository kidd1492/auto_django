import os, sys, subprocess

original_dir = os.getcwd()


def main():
    args = sys.argv
    
    if len(args) == 1:
        create_project()

    elif len(args) == 2:
        app_name = args[1]
        create_project()
        create_app(app_name)
        app_files(app_name, "default")
        settings(app_name)
        create_project_url(app_name)

    elif len(args) == 3:
        app_types = ["reservation", "store", "inventory", "task"]
        command = args[1]
        name = args[2]

        if command == "add_app":  # NEW: Allow adding apps to an existing project
            app_type = name
            app_name = args[2]
            if app_type in app_types:
                create_app(app_name)
                app_files(app_name, app_type)
                settings(app_name)
                project_url(app_name)
                create_database(app_name, app_type)
                print(f"App '{app_name}' added successfully to project!")
            else:
                print("Please Select One:")
                for type in app_types:print(type)

        elif command in app_types:
            create_project()
            create_app(name)
            app_files(name, command)
            settings(name)
            create_project_url(name)
            create_database(name, command)
        else:
            print("Please Select One:")
            for type in app_types:print(type)       


    elif len(args) == 4:
        function_call = args[1]
        name = args[2]
        template_name = args[3]

        if function_call == "template":
            template(name, template_name)
            apps_url(name, template_name)


def create_project():
    os.makedirs("django_app", exist_ok=True)
    os.chdir("django_app")
    
    if os.name == "nt":  # Windows
        subprocess.run(["python", "-m", "venv","venv"], check=True)
        venv_activate = r"venv\Scripts\activate"
    else:  # macOS/Linux
        subprocess.run(["python3", "-m", "venv","venv"], check=True)
        venv_activate = "source venv/bin/activate"

    subprocess.run(f"{venv_activate} && pip install django && django-admin startproject myproject", shell=True)
    os.chdir(original_dir)


def create_app(app_name):
    os.chdir("django_app/myproject")
    subprocess.run(['python', 'manage.py', 'startapp', f'{app_name}'])
    os.makedirs(f"{app_name}/templates/{app_name}", exist_ok=True)
    os.makedirs(f"{app_name}/static/{app_name}", exist_ok=True)
    os.chdir(original_dir)

   
def create_project_url(app_name):
    os.chdir("django_app/myproject")
    url = os.path.join("myproject", "urls.py")
    with open(url, "w") as  prourl_file: 

        prourl_file.write("from django.contrib import admin\n")
        prourl_file.write("from django.urls import path, include\n\n")
        prourl_file.write("urlpatterns = [\n")
        prourl_file.write("    path('admin/', admin.site.urls),\n")
        prourl_file.write(f"    path('{app_name}/', include('{app_name}.urls')),\n")
        prourl_file.write("]\n")
    os.chdir(original_dir)

    
def app_files(app_name, directory):
    #index.html
    os.chdir("django_app/myproject")
    index = os.path.join(app_name, "templates", app_name, "index.html")
    with open(index, "w") as index_file:
        index_file.write("{% extends \"" + app_name + "/layout.html\" %}\n\n")
        index_file.write("{% block body %}\n")
        index_file.write("    <h1>Index</h1>\n")
        index_file.write("{% endblock %}\n")
       
    filepath = os.path.join(f'{app_name}', "urls.py")
    with open(filepath, "w") as url_file:
        #Write the Python code to the file
        url_file.write("from django.urls import path\n")
        url_file.write("from . import views\n\n")
        url_file.write(f"urlpatterns = [\n")
        url_file.write(f"    path('', views.index, name='index'),\n")
        url_file.write(f"]\n")

    views = os.path.join(f'{app_name}', "views.py")
    with open(views, "w") as view_file:
        # Write the Python code to the file
        view_file.write("from django.contrib import admin\n")
        view_file.write("from django.shortcuts import render\n\n")
        view_file.write("# Create your views here.\n")
        view_file.write("def index(request):\n")
        view_file.write(f"    return render(request, '{app_name}/index.html')")
    os.chdir(original_dir)
   

    layout_path =  os.path.join("django_app", "myproject", app_name, "templates", app_name, "layout.html")
    layout_file_to_copy = os.path.join(directory, "layout.html")
    os.system(f"copy {layout_file_to_copy} {layout_path}")

    copy_filepath = os.path.join("django_app", "myproject", app_name, "templates", app_name, "copy.html")
    text = "    <link href=\"{% static '" + app_name + "/style.css' %}\" rel=\"stylesheet\">\n"
    search_term = "<head>"
    insert_line(layout_path, copy_filepath, search_term, text)

    css_file_path = os.path.join("django_app", "myproject", app_name, 'static', app_name, 'style.css')
    css_to_copy_file = os.path.join(directory, "style.css")
    os.system(f"copy {css_to_copy_file} {css_file_path}")


def create_database(app_name, directory):
    models_to_copy_file = os.path.join(directory, "models.py")
    models_destination = os.path.join("django_app", "myproject", app_name, "models.py")
    os.system(f"copy {models_to_copy_file} {models_destination}")

    admin_to_copy_file = os.path.join(directory, "admin.py")
    admin_destination = os.path.join("django_app", "myproject", app_name, "admin.py")
    os.system(f"copy {admin_to_copy_file} {admin_destination}")

    os.chdir("django_app/myproject")
    subprocess.run(['python', 'manage.py', 'makemigrations'])
    subprocess.run(['python', 'manage.py', 'migrate'])
    os.chdir(original_dir)


def settings(app_name):
    
    filepath = os.path.join("django_app", "myproject", "myproject", "settings.py")
    copy_filepath = os.path.join("django_app", "myproject", "myproject", "copy.py")
    search_term ="INSTALLED_APPS = ["
    text = f"    '{app_name}',\n"
    insert_line(filepath, copy_filepath, search_term, text)
    

def project_url(app_name):
   
    search_term = "urlpatterns = ["
    filepath = os.path.join("django_app", "myproject", "myproject", "urls.py")
    copy_filepath = os.path.join("django_app", "myproject", "myproject", "copy.py")
    text = f"    path('{app_name}/', include('{app_name}.urls')),\n"
    insert_line(filepath, copy_filepath, search_term, text)


def apps_url(app_name, template_name):
    os.chdir("django_app/myproject")
    copy_filepath = os.path.join(app_name, "copy.txt")
    search_term = "urlpatterns = ["
    filepath = os.path.join(app_name, "urls.py")
    text = f"    path('{template_name}', views.{template_name}, name='{template_name}'),\n"
    insert_line(filepath, copy_filepath, search_term, text)
    os.chdir(original_dir)


def template(app_name, template_name):
    os.chdir("django_app/myproject")
    filepath = os.path.join(app_name, "templates", app_name, f"{template_name}.html")

    with open(filepath, "w") as template:
        template.write("{% extends '" + app_name + "/layout.html' %}\n\n")
        template.write("{% block title %}"+template_name+"{% endblock %}\n")
        template.write("{% block body %}\n")
        template.write("    <h1>Index</h1>\n")
        template.write("{% endblock %}\n")

    
    filepath = os.path.join(app_name, "views.py")

    with open(filepath, "a") as views:
        views.write(f"\n\ndef {template_name}(request):\n")
        views.write(f"    return render(request, '{app_name}/{template_name}.html')")
    os.chdir(original_dir)


def insert_line(filepath, copy_filepath, search_term, text):
    with open(filepath, "r") as input_file:  
        with open(copy_filepath, "w") as copy:
            for line in input_file:
                copy.write(line)
                if search_term in line:
                    copy.write(text)

    os.remove(filepath)
    os.rename(copy_filepath, filepath) 
    os.chdir(original_dir)


if __name__ == "__main__":
    main()
