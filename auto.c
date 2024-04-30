#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int project(void);
int create_app(char* app_name);
int app_files(char* app_name);
int create_files(char* app_name);
int template(char* app_name, char* template_name);
int settings(char* app_name);
int project_url(char* app_name);
int apps_url(char* app_name, char* template_name);


int main(int argc, char *argv[])
{

    // Ensure the correct number of arguments are provided
    if (argc == 1) {
        project();
        return 0;
    }

    if (argc == 2) {
        char* name = argv[1];
        project();
        create_app(name);
        app_files(name);
        create_files(name);
        settings(name);
        return 0;
    }

    if (argc == 3) {
        char* function_call = argv[1];
        char* name = argv[2];
        if (strcmp(function_call, "create_app") == 0) { // Use strcmp() for comparison
            create_app(name);
            app_files(name);
            settings(name);
            project_url(name);
        }
        return 0;
    } 

    if (argc == 4) {
        char* function_call = argv[1];
        char* name = argv[2];
        char* template_name = argv[3];

        if (strcmp(function_call, "template") == 0) { // Use strcmp() for comparison
            template(name, template_name);
            apps_url(name, template_name);
        }
        return 0;
    }  
    return 1;
}

// Set up venv, pip install django, starts django project
int project(void) {
    int status;
    // mkdir and setup venv
    status = system("mkdir django_app && cd django_app && python -m venv venv");

    // Activate the virtual environment and install Django
    status = system("django_app\\venv\\Scripts\\activate.bat && pip install django");

    // Start a new Django project
    status = system("django_app\\venv\\Scripts\\activate.bat && cd django_app && django-admin startproject myproject");
    return status;
}
    
   
// Start Django app
int create_app(char* app_name) {
    int status;
    char command[256];
    //Start app
    sprintf(command,"django_app\\venv\\Scripts\\activate.bat && cd django_app/myproject && python manage.py startapp %s", app_name);
    status = system(command);

    //Creates templates and static
    sprintf(command, "cd django_app/myproject/%s && mkdir templates\\%s && mkdir static\\%s", app_name, app_name, app_name);
    status = system(command);
    return status;
}
    

//starter files
int app_files(char* app_name){

    char filepath[256];
    FILE *css_file;
    FILE *layout_file;
    FILE *index_file;
    FILE *url_file;
    FILE *view_file;
    // Filepath for style.css
    sprintf(filepath, "django_app/myproject\\%s\\static\\%s\\style.css", app_name, app_name);

    // Open the file for writing
    css_file = fopen(filepath, "w");
    fprintf(css_file, "body {\n    background-color: green;\n}\n");
    fclose(css_file);

    // Filepath for layout.html
    sprintf(filepath, "django_app/myproject\\%s\\templates\\%s\\layout.html", app_name, app_name);

    // Open the file for writing
    layout_file = fopen(filepath, "w");
    // Write the HTML content to the file
    fprintf(layout_file, "{%% load static %%}\n\n");
    fprintf(layout_file, "<!DOCTYPE html>\n");
    fprintf(layout_file, "<html lang=\"en\">\n");
    fprintf(layout_file, "<head>\n");
    fprintf(layout_file, "    <meta charset=\"UTF-8\">\n");
    fprintf(layout_file, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(layout_file, "    <link href=\"{%% static '%s/style.css' %%}\" rel=\"stylesheet\">\n", app_name);
    fprintf(layout_file, "    <title>{%% block title %%}{%% endblock %%}</title>\n");
    fprintf(layout_file, "</head>\n");
    fprintf(layout_file, "<body>\n");
    fprintf(layout_file, "    {%% block body %%}\n");
    fprintf(layout_file, "    \n");
    fprintf(layout_file, "    {%% endblock %%}\n");
    fprintf(layout_file, "</body>\n");
    fprintf(layout_file, "</html>\n");
    fclose(layout_file);

    // Filepath for index.html
    sprintf(filepath, "django_app/myproject/%s/templates/%s/index.html", app_name, app_name);

    // Open the file for writing
    index_file = fopen(filepath, "w");

    // Write the HTML content to the file
    fprintf(index_file, "{%% extends \"%s/layout.html\" %%}\n\n", app_name);
    fprintf(index_file, "{%% block body %%}\n");
    fprintf(index_file, "    <h1>Index</h1>\n");
    fprintf(index_file, "{%% endblock %%}\n");
    fclose(index_file);

     // Filepath for urls.py
    sprintf(filepath, "django_app/myproject\\%s/urls.py", app_name, app_name);

    // Open the file for writing
    url_file = fopen(filepath, "w");

    // Write the Python code to the file
    fprintf(url_file, "from django.urls import path\n");
    fprintf(url_file, "from . import views\n\n");
    fprintf(url_file, "urlpatterns = [\n");
    fprintf(url_file, "    path(\"\", views.index, name=\"index\"),\n");
    fprintf(url_file, "]\n");
    fclose(url_file);

    // Filepath for views.py
    sprintf(filepath, "django_app/myproject\\%s/views.py", app_name);

    // Open the file for writing
    view_file = fopen(filepath, "w");

    // Write the Python code to the file
    fprintf(view_file, "from django.contrib import admin\n");
    fprintf(view_file, "from django.shortcuts import render\n\n");
    fprintf(view_file, "# Create your views here.\n");
    fprintf(view_file, "def index(request):\n");
    fprintf(view_file, "    return render(request, '%s/index.html')", app_name);
    fclose(view_file);

    return 0;
}


int create_files(char* app_name){
    
    char filepath[256];
    FILE *prourl_file;
    
    // Filepath for prourls.py
    sprintf(filepath, "django_app/myproject/myproject/urls.py");

    // Open the file for writing
    prourl_file = fopen(filepath, "w");

    // Write the Python code to the file
    fprintf(prourl_file, "from django.contrib import admin\n");
    fprintf(prourl_file, "from django.urls import path, include\n\n");
    fprintf(prourl_file, "urlpatterns = [\n");
    fprintf(prourl_file, "    path('admin/', admin.site.urls),\n");
    fprintf(prourl_file, "    path('%s/', include(\"%s.urls\")),\n", app_name, app_name);
    fprintf(prourl_file, "]\n");
    fclose(prourl_file);
    return 0;
}


int template(char* app_name, char* template_name){
    char filepath[256];
    FILE *template;
    FILE * views;
    // Filepath for index.html
    sprintf(filepath, "django_app/myproject/%s/templates/%s/%s.html", app_name, app_name, template_name);

    // Open the file for writing
    template = fopen(filepath, "w");

    // Write the HTML content to the file
    fprintf(template, "{%% extends \"%s/layout.html\" %%}\n\n", app_name);
    fprintf(template, "{%% block title %%}%s{%% endblock %%}\n", template_name);
    fprintf(template, "{%% block body %%}\n");
    fprintf(template, "    <h1>Index</h1>\n");
    fprintf(template, "{%% endblock %%}\n");
    fclose(template);

    //update views when adding template
    // Filepath for views.py
    sprintf(filepath, "django_app/myproject\\%s/views.py", app_name);

    // Open the file for writing
    views = fopen(filepath, "a");

    fprintf(views, "\n\ndef %s(request):\n", template_name);
    fprintf(views, "    return render(request, '%s/%s.html')", app_name, template_name);
    fclose(views);
    return 0;    
}


int settings(char* app_name) {

    char line[1024];
    char filepath[256];
    char search_term[] ="INSTALLED_APPS = [";

    // Filepath settings.py
    sprintf(filepath, "django_app/myproject/myproject/settings.py");
    FILE *input_file = fopen(filepath, "r");

    char copy_filepath[256];
    sprintf(copy_filepath, "django_app/myproject/myproject/copy.txt");
    FILE *copy = fopen(copy_filepath, "w");

    if (input_file == NULL) {
    perror("Error opening input file");
    return 1;
    }

    while (fgets(line, sizeof(line), input_file)) {
        fprintf(copy, line);
        if (strstr(line, search_term)) {
            fprintf(copy, "    '%s',\n", app_name);
            continue;
        }
    }

    fclose(input_file);
    fclose(copy);
    remove("django_app/myproject/myproject/settings.py");
    rename("django_app/myproject/myproject/copy.txt", "django_app/myproject/myproject/settings.py");
    
    return 0;
}


int project_url(char* app_name) {

    char line[1024];
    char filepath[256];
    char search_term[] = "urlpatterns = [";

    // Filepath settings.py
    sprintf(filepath, "django_app/myproject/myproject/urls.py");
    FILE *input_file = fopen(filepath, "r");

    char copy_filepath[256];
    sprintf(copy_filepath, "django_app/myproject/myproject/copy.txt");
    FILE *copy = fopen(copy_filepath, "w");

    if (input_file == NULL) {
    perror("Error opening input file");
    return 1;
    }

    while (fgets(line, sizeof(line), input_file)) {
        fprintf(copy, line);
        if (strstr(line, search_term)) {
            fprintf(copy, "    path('%s/', include(\"%s.urls\")),\n", app_name, app_name);
            continue;
        }
    }

    fclose(input_file);
    fclose(copy);
    remove("django_app/myproject/myproject/urls.py");
    rename("django_app/myproject/myproject/copy.txt", "django_app/myproject/myproject/urls.py");
    
    return 0;
}


int apps_url(char* app_name, char* template_name) {

    char line[1024];
    char filepath[256];
    char search_term[] = "urlpatterns = [";

    // Filepath apps url.py
    // Filepath for urls.py
    sprintf(filepath, "django_app/myproject\\%s/urls.py", app_name);
    FILE *input_file = fopen(filepath, "r");

    char copy_filepath[256];
    sprintf(copy_filepath, "django_app/myproject\\%s/copy.txt", app_name);
    FILE *copy = fopen(copy_filepath, "w");

    if (input_file == NULL) {
    perror("Error opening input file");
    return 1;
    }

    while (fgets(line, sizeof(line), input_file)) {
        fprintf(copy, line);
        if (strstr(line, search_term)) {
            fprintf(copy, "    path('%s', views.%s, name='%s'),\n", template_name, template_name, template_name);
            continue;
        }
    }

    fclose(input_file);
    fclose(copy);
    remove(filepath);
    rename(copy_filepath, filepath);
    
    return 0;
}
