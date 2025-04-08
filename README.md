# Django Automation Tool

## Overview
This tool is a command-line utility that automates the setup of a new Django project and app, streamlining the development process. I created the program in C and Python. I created another branch that will allow the user to create a default app or an app with a database that is automatically created and registered in the admin section.

## Features
Processes Automated: 
- Sets up a new Django project directory django_app/
- Steps in django_app/ creates virtual.
- Activates venv
- pip install django
- Generates a new app with necessary directories for templates and static files.
- Creates essential starter files like `style.css`, `layout.html`, and `index.html`.
- Configures URL routing for the app.
- Adds custom templates for different views.
- Adds the app to the settings file.


## Getting Started
1. Clone the repository.
2. run the script then cd django_app/myproject, run python manage.py runserver
3. go to server url 127.0.0:8000/app_name

## C Usage
2. Compile the C code using a C compiler (e.g., `gcc`).
3. Run the compiled executable with the appropriate arguments.

- To create a new Django project:
  - `./auto`
- To create a new app within the project:
  - `./auto app_name`
- To create additional apps:
  - `./auto create_app app_name` 
- To create template:
  - `./auto template app_name template_name`

## Python Usage
- Create project with app(Defult app with simple index layout css):
  - `python auto_django.py app_name`
- Create project with app template["reservsation", "task"]
- This will create a database migrate and install templates css admin register
  - ex: `python auto_django.py task app_name`
- Create additional apps:
  - `python auto_django.py add_app app_type`
- Create a new template html
  - `python auto_django.py template app_name template_name`