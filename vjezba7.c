#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int print_file_info(const char *name)
{
	struct stat sb;
	struct passwd *pw;
	struct group  *gr;

	char date_time[128];

	if (stat(name, &sb) == -1)
		return EXIT_FAILURE;

	pw = getpwuid(sb.st_uid);
	gr = getgrgid(sb.st_gid);

	strftime(date_time, 128, "%b %d %R", localtime(&(sb.st_ctime)));

	printf(
		"%c%c%c%c%c%c%c%c%c%c %2ju %-10s %-10s %5jd %s %-10s \n",
		S_ISDIR(sb.st_mode)	? 'd' : '-',

		sb.st_mode & S_IRUSR	? 'r' : '-',
		sb.st_mode & S_IWUSR	? 'w' : '-',
		sb.st_mode & S_IXUSR	? 'x' : '-',

		sb.st_mode & S_IRGRP	? 'r' : '-',
                sb.st_mode & S_IWGRP	? 'w' : '-',
                sb.st_mode & S_IXGRP	? 'x' : '-',

		sb.st_mode & S_IROTH	? 'r' : '-',
                sb.st_mode & S_IWOTH	? 'w' : '-',
                sb.st_mode & S_IXOTH	? 'x' : '-',

		sb.st_nlink,

		pw->pw_name,
		gr->gr_name,

		sb.st_size,

		date_time,
		name
	);

	return EXIT_SUCCESS;
}

int list_directory(const char *path)
{
	DIR *d;
	struct dirent *dir;

	printf("Opening folder %s\n", path);
	d = opendir(path);
	if (!d) return EXIT_FAILURE;

	while((dir = readdir(d)) != NULL)
		print_file_info(dir->d_name);

	return EXIT_SUCCESS;
}

int list_file(const char *path)
{
	DIR *d;
	struct dirent *dir;

	d = opendir(".");
	if (!d) return EXIT_FAILURE;

	while((dir = readdir(d)) != NULL)
		if (!strcmp(dir->d_name, path))
			print_file_info(dir->d_name);

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	struct stat sb;

	if (argc == 1) {
		//printf("No argument given\n");

		list_directory(".");
	} else if (argc == 2) {
		//printf("One argument given\n");

		// Is it a file or a folder?
		if(stat(argv[1], &sb) == -1)
			return EXIT_FAILURE;

		if(S_ISDIR(sb.st_mode)) {
			printf("Is folder\n");
			list_directory(argv[1]);
		}
		else {
			printf("Is File\n");
			list_file(argv[1]);
		}
	} else {
		printf("Usage: %s <file/folder>\n", argv[0]);
		printf("<file/folder is optional>\n");
	}

	return EXIT_SUCCESS;
}
