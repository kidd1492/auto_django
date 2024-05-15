#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int project(void);
int create_app(char* app_name);
int app_files(char* app_name, const char* app_type);
int prourl(char* app_name);

int template(char* app_name, char* template_name);

int project_url(char* app_name);
int apps_url(char* app_name, char* template_name);

int layouts(char* app_name, const char* source_path, const char* destination_path, char* search_term);
int insert(char* app_name, const char* source_path, const char* destination_path, char* search_term, char* text);
int copy_files(const char* source_path, const char* destination_path);

int copy_models(char* app_name, const char* app_type);
int create_db(void);

int main(int argc, char *argv[])
{

    // Ensure the correct number of arguments are provided
    if (argc == 1) {
        project();
        return 0;
    }

    if (argc == 2) {
        char* name = argv[1];
        char app_type[10];
        sprintf(app_type, "default");

        project();
        create_app(name);
        
        app_files(name, app_type);
        prourl(name);        
        return 0;
    }

    if (argc == 3) {
        char* function_call = argv[1];
        
        if (strcmp(function_call, "create_app") == 0) { // Use strcmp() for comparison
            char* name = argv[2];
            char app_type[10];
            sprintf(app_type, "default");

            create_app(name);
            app_files(name, app_type);       
            project_url(name);
            return 0;
        }

        else{
            char* name = argv[1];
            char* app_type = argv[2];
            
            project();
            create_app(name);
            app_files(name, app_type);
            prourl(name);
            copy_models(name, app_type);
            create_db();
        return 0;
        }
        
    } 

    if (argc == 4) {
        char* function_call = argv[1];
        

        if (strcmp(function_call, "template") == 0) { // Use strcmp() for comparison
            char* name = argv[2];
            char* template_name = argv[3];
            template(name, template_name);
            apps_url(name, template_name);
            return 0;
        }
        

        if (strcmp(function_call, "create_app") == 0) { // Use strcmp() for comparison
            char* name = argv[2];
            char* app_type = argv[3];

            create_app(name);
            app_files(name, app_type);       
            project_url(name);
            copy_models(name, app_type);
            create_db();
            
            return 0;
        }
        
    }  
    return 1;
}

// Set up venv, pip install django, starts django project
int project(void) {
    // Run all commands in one shell
    int status = system(
        "mkdir django_app && cd django_app && "
        "python -m venv venv && "
        "venv\\Scripts\\activate.bat && "
        "pip install django && "
        "django-admin startproject myproject"
    );
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
    sprintf(command, "cd django_app/myproject/%s && mkdir templates\\%s && mkdir static\\%s && cd static\\%s && mkdir images", app_name, app_name, app_name, app_name);
    status = system(command);
    return status;
}
    

//starter files
int app_files(char* app_name, const char* app_type){
    char filepath[256]; 
    char source_path[256];
    char destination_path[256];
    char search_term[256];
    char text[256];

    // Create stylesheet 
    //need to update directory to store
    sprintf(source_path, "files/%s/style.css", app_type);
    sprintf(destination_path, "django_app/myproject/%s/static/%s/style.css", app_name, app_name);
    copy_files(source_path, destination_path);

    // Create app's urls.py file
    sprintf(source_path, "files/app_files/urls.py");
    sprintf(destination_path, "django_app/myproject/%s/urls.py", app_name);
    copy_files(source_path, destination_path);

    // Create the index html by calling templates
    char index[10];
    sprintf(index, "index");
    template(app_name, index);

    // Filepath for layout.html 
    //need to update directory to store
    sprintf(source_path, "files/%s/layout.html", app_type);
    sprintf(destination_path, "django_app/myproject/%s/templates/%s/layout.html", app_name, app_name);
    sprintf(search_term, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    layouts(app_name, source_path, destination_path, search_term);

    // file paths prject settings.py
    sprintf(source_path, "django_app/myproject/myproject/settings.py");
    sprintf(destination_path, "django_app/myproject/myproject/copy.txt");
    sprintf(search_term, "INSTALLED_APPS = [");
    sprintf(text, "    '%s',\n", app_name);

    insert(app_name, source_path, destination_path, search_term, text);


    // Filepath for views.py
    sprintf(filepath, "django_app/myproject\\%s/views.py", app_name);

    // Open the file for writing
    FILE *view_file;
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

int prourl(char* app_name){
    char filepath[256];
     //Filepath for prourls.py
    sprintf(filepath, "django_app/myproject/myproject/urls.py");

    // Open the file for writing
    FILE *prourl_file;
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


int layouts(char* app_name, const char* source_path, const char* destination_path, char* search_term) {

    char line[1024];
    FILE *source_file = fopen(source_path, "r");
    FILE *destination_file = fopen(destination_path, "w");

    while (fgets(line, sizeof(line), source_file)) {
        fprintf(destination_file, line);
        if (strstr(line, search_term)) {
            fprintf(destination_file, "    <link href=\"{%% static '%s/style.css' %%}\" rel=\"stylesheet\">\n", app_name);
            fprintf(destination_file, "    <script src=\"{%% static '%s/jscript.js' %%}\"></script>\n", app_name);
            continue;
        }
    }

    fclose(source_file);
    fclose(destination_file);
    return 0;
}


int project_url(char* app_name) {
    char source_path[256];
    char destination_path[256];
    char search_term[256];
    char text[256];

    // Filepath for project urls.py
    sprintf(source_path, "django_app/myproject/myproject/urls.py");
    sprintf(destination_path, "django_app/myproject/myproject/copy.txt");
    sprintf(search_term, "urlpatterns = [");
    sprintf(text, "    path('%s/', include(\"%s.urls\")),\n", app_name, app_name);
    insert(app_name, source_path, destination_path, search_term, text);
    
    return 0;
}


int apps_url(char* app_name, char* template_name) {
    char source_path[256];
    char destination_path[256];
    char search_term[256];
    char text[256];

    // Filepath for project urls.py
    sprintf(source_path, "django_app/myproject\\%s/urls.py", app_name);
    sprintf(destination_path, "django_app/myproject\\%s/copy.txt", app_name);
    sprintf(search_term, "urlpatterns = [");
    sprintf(text, "    path('%s', views.%s, name='%s'),\n", template_name, template_name, template_name);
    insert(app_name, source_path, destination_path, search_term, text);
    
    return 0;
}


int copy_files(const char* source_path, const char* destination_path) {

    FILE *source_file = fopen(source_path, "r");
    FILE *destination_file = fopen(destination_path, "w");

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
        fwrite(buffer, 1, bytes, destination_file);
    }
    
    fclose(source_file);
    fclose(destination_file);
    return 0;
}


int insert(char* app_name, const char* source_path, const char* destination_path, char* search_term, char* text) {

    char line[1024];
    FILE *source_file = fopen(source_path, "r");
    FILE *destination_file = fopen(destination_path, "w");

    while (fgets(line, sizeof(line), source_file)) {
        fprintf(destination_file, line);
        if (strstr(line, search_term)) {
            fprintf(destination_file, text);
            continue;
        }
    }

    fclose(source_file);
    fclose(destination_file);
    remove(source_path);
    rename(destination_path, source_path);
    return 0;
}


int copy_models(char* app_name, const char* app_type){
    char source_path[256];
    char destination_path[256];

    // Create models.py 
    sprintf(source_path, "files/%s/models.py", app_type);
    sprintf(destination_path, "django_app/myproject/%s/models.py", app_name);
    copy_files(source_path, destination_path);

    // Create admin.py 
    sprintf(source_path, "files/%s/admin.py", app_type);
    sprintf(destination_path, "django_app/myproject/%s/admin.py", app_name);
    copy_files(source_path, destination_path);
    return 0;

}


int create_db(void){
    int status;
    char command[256];
    // Make migrations
    sprintf(command,"django_app\\venv\\Scripts\\activate.bat && cd django_app/myproject && python manage.py makemigrations && python manage.py migrate");
    status = system(command);

    return 0;
}