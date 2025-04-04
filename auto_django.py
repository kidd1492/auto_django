import os, sys, subprocess

original_dir = os.getcwd()


def main():
    args = sys.argv
    if len(args) == 1:
        create_project()

    if len(args) == 2:
        app_name = args[1]
        create_project()
        create_app(app_name)
        app_files(app_name)
        create_files(app_name)
        settings(app_name)

    if len(args) == 3:
        function_call = args[1]
        name = args[2]
        if function_call == "create_app":
            create_app(name)
            app_files(name)
            settings(name)
            project_url(name)

    if len(args) == 4:
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


def app_files(app_name):
    os.chdir("django_app/myproject")
    index = os.path.join(app_name, "templates", app_name, "index.html")
    with open(index, "w") as index_file:
        index_file.write("{% extends \"" + app_name + "/layout.html\" %}\n\n")
        index_file.write("{% block body %}\n")
        index_file.write("    <h1>Index</h1>\n")
        index_file.write("{% endblock %}\n")
    
   
    layout_path =  os.path.join(app_name, "templates", app_name, "layout.html")
    with open(layout_path, "w") as layout_file:
        
        layout_file.write("{% load static %}\n\n")
        layout_file.write("<!DOCTYPE html>\n")
        layout_file.write("<html lang=\"en\">\n")
        layout_file.write("<head>\n")
        layout_file.write("    <meta charset=\"UTF-8\">\n")
        layout_file.write("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n")
        layout_file.write("    <link href=\"{% static '" + app_name + "/style.css' %}\" rel=\"stylesheet\">\n")
        layout_file.write("    <title>{% block title %}{% endblock %}</title>\n")
        layout_file.write("</head>\n")
        layout_file.write("<body>\n")
        layout_file.write("    {% block body %}\n")
        layout_file.write("    \n")
        layout_file.write("    {% endblock %}\n")
        layout_file.write("</body>\n")
        layout_file.write("</html>\n")
    

    css_file_path = os.path.join(app_name, 'static', app_name, 'style.css')
    with open(css_file_path, "w") as css_file:
        css_file.write("body {\n    background-color: green;\n}\n")


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

    
def create_files(app_name):
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


def settings(app_name):
    os.chdir("django_app/myproject")
    settings_path = os.path.join("myproject", "settings.py")
    copy_path = os.path.join("myproject", "copy.py")
    search_term ="INSTALLED_APPS = ["

    with open(settings_path, "r", encoding="UTF-8") as settings_file:
        with open(copy_path, "w") as copy:
            for line in settings_file:
                copy.write(line)
                if search_term in line:
                    copy.write(f"    '{app_name}',\n")
 
    os.remove(settings_path)
    os.rename(copy_path, settings_path)
    os.chdir(original_dir)


def project_url(app_name):
    os.chdir("django_app/myproject")
    search_term = "urlpatterns = ["
    url_path = os.path.join("myproject", "urls.py")
    copy_filepath = os.path.join("myproject", "copy.py")

    with open(url_path, "r") as url_file:      
        with open(copy_filepath, "w") as copy:
            for line in url_file:
                copy.write(line)
                if search_term in line:
                    copy.write(f"    path('{app_name}/', include('{app_name}.urls')),\n")
              
    os.remove(url_path)
    os.rename(copy_filepath, url_path)
    os.chdir(original_dir)


def apps_url(app_name, template_name):
    os.chdir("django_app/myproject")
    copy_filepath = os.path.join(app_name, "copy.txt")
    search_term = "urlpatterns = ["
    filepath = os.path.join(app_name, "urls.py")

    with open(filepath, "r") as input_file:  
        with open(copy_filepath, "w") as copy:
            for line in input_file:
                copy.write(line)
                if search_term in line:
                    copy.write(f"    path('{template_name}', views.{template_name}, name='{template_name}'),\n")

    os.remove(filepath)
    os.rename(copy_filepath, filepath)
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


if __name__ == "__main__":
    main()
