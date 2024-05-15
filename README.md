# Django Automation Tool

## Overview
This tool is a command-line utility that automates the setup of a new Django project and app, streamlining the development process. In this version I set up a directory to hold a few files to transfer into the Django app. This includes a models.py file and the admin update. Then it will run makemigrations and migrate to create the database. Run createsuperuser and set up the account. when you log into the admin area the tables will be registered.

## Features
- Sets up a new Django project with a virtual environment and installs Django.
- Generates a new app with necessary directories for templates and static files.
- Creates essential starter files like `style.css`, `layout.html`, and `index.html`.
- Configures URL routing for the app.
- Adds custom templates for different views.

## Getting Started
1. Clone the repository.
2. Compile the C code using a C compiler (e.g., `gcc`).
3. Run the compiled executable with the appropriate arguments.

## Usage
- To create a new Django project with NO app:
  - ./auto

- To create a new Django project with a default app:
  - ./auto app_name

- To create an additional app with default settings:
  - ./auto create_app app_name

- To create an app with a specific type (e.g., 'res' for a reservation app):
  - ./auto app_name app_type

  - If `app_type` is 'res', the program will copy `models.py`, update `admin.py`, and run migrations to create and set up the database.

- To add a custom template to an app:
  - ./auto template app_name template_name



  
