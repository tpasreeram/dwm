# include <stdio.h>
# include <string.h>

void exitdwm ()
{
# if							   \
	defined S_LOCK				|| \
	defined S_RESTART_DWM		|| \
	defined S_EXIT				|| \
	defined S_REBOOT			|| \
	defined S_SHUTDOWN			|| \
	defined S_LOCK_ICON			|| \
	defined S_RESTART_DWM_ICON	|| \
	defined S_EXIT_ICON			|| \
	defined S_REBOOT_ICON		|| \
	defined S_SHUTDOWN_ICON		|| \
	defined S_FORMAT			|| \
	defined S_FORMAT_CLEAR
# error (conflicting macro names)
# endif

# define S_LOCK "Sleep"
# define S_RESTART_DWM "Restart DWM"
# define S_EXIT "Exit"
# define S_REBOOT "Reboot"
# define S_SHUTDOWN "Shutdown"
# define S_LOCK_ICON "󰤄"			// <= FontAwesome icons
# define S_RESTART_DWM_ICON ""
# define S_EXIT_ICON ""
# define S_REBOOT_ICON ""
# define S_SHUTDOWN_ICON ""

# define S_FORMAT(ACTION) S_##ACTION##_ICON " " S_##ACTION
# define S_FORMAT_CLEAR "sed 's/^..//'"

	FILE * exit_menu = popen (
		"echo \""
			S_FORMAT (LOCK) "\n"
			S_FORMAT (RESTART_DWM) "\n"
			S_FORMAT (EXIT) "\n"
			S_FORMAT (REBOOT) "\n"
			S_FORMAT (SHUTDOWN)
			"\" | dmenu -p exit: | " S_FORMAT_CLEAR
		,
		"r"
	);

	char exit_action [16];

	if (
		exit_menu == NULL ||
		fscanf (exit_menu, "%15[a-zA-Z -]", exit_action) == EOF
	) {
		fputs ("Error. Failure in exit_dwm.", stderr);
		goto close_streams;
	}

	if (strcmp (exit_action, S_LOCK) == 0) system ("systemctl suspend");
	else if (strcmp (exit_action, S_RESTART_DWM) == 0) {
    quit (& (const Arg) {1});
    quit (& (const Arg) {1});
  }
	else if (strcmp (exit_action, S_EXIT) == 0) {
    quit (& (const Arg) {0});
    quit (& (const Arg) {0});
  }
	else if (strcmp (exit_action, S_REBOOT) == 0) system ("systemctl reboot");
	else if (strcmp (exit_action, S_SHUTDOWN) == 0) system ("systemctl poweroff -i");

close_streams:
	pclose (exit_menu);

# undef S_LOCK
# undef S_RESTART_DWM
# undef S_EXIT
# undef S_REBOOT
# undef S_SHUTDOWN
# undef S_LOCK_ICON
# undef S_RESTART_DWM_ICON
# undef S_EXIT_ICON
# undef S_REBOOT_ICON
# undef S_SHUTDOWN_ICON
# undef S_FORMAT
# undef S_FORMAT_CLEAR
}
