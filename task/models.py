from django.db import models

class User(models.Model):
    username = models.CharField(max_length=150, unique=True)
    email = models.EmailField(unique=True)
    password = models.CharField(max_length=100)  # Consider using Django's built-in authentication
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.username

class Project(models.Model):
    name = models.CharField(max_length=200)
    description = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.name

class Task(models.Model):
    project = models.ForeignKey(Project, on_delete=models.CASCADE)
    title = models.CharField(max_length=200)
    description = models.TextField(blank=True, null=True)
    deadline = models.DateField(blank=True, null=True)
    status_choices = [('TODO', 'To Do'), ('IN_PROGRESS', 'In Progress'), ('DONE', 'Done')]
    status = models.CharField(max_length=20, choices=status_choices, default='TODO')
    assigned_to = models.ForeignKey(User, on_delete=models.SET_NULL, null=True, blank=True)

    def __str__(self):
        return f"{self.title} - {self.status}"