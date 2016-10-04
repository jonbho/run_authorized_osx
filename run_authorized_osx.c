#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Security/Security.h>

int run_authorized(int argc, char *prog_and_args[])
{
    //printf("Going to run: ");
    //for (int i = 0; i < argc; i++)
    //    printf("%s ", prog_and_args[i]);
    //printf("\n");

    // Create authorization reference
    OSStatus status;
    AuthorizationRef authorizationRef;

    // AuthorizationCreate and pass NULL as the initial
    // AuthorizationRights set so that the AuthorizationRef gets created
    // successfully, and then later call AuthorizationCopyRights to
    // determine or extend the allowable rights.
    // http://developer.apple.com/qa/qa2001/qa1172.html
    status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authorizationRef);
    if (status != errAuthorizationSuccess)
    {
        //NSLog(@"Error Creating Initial Authorization: %d", status);
        return -1;
    }

    // kAuthorizationRightExecute == "system.privilege.admin"
    AuthorizationItem right = {kAuthorizationRightExecute, 0, NULL, 0};
    AuthorizationRights rights = {1, &right};
    AuthorizationFlags flags = kAuthorizationFlagDefaults | kAuthorizationFlagInteractionAllowed |
                                kAuthorizationFlagPreAuthorize | kAuthorizationFlagExtendRights;

    // Call AuthorizationCopyRights to determine or extend the allowable rights.
    status = AuthorizationCopyRights(authorizationRef, &rights, NULL, flags, NULL);
    if (status != errAuthorizationSuccess)
    {
        //NSLog(@"Copy Rights Unsuccessful: %d", status);
        return -1;
    }

    // Prepare arguments, needs memory
    char **args = malloc(argc * sizeof(char*));
    for (int i = 1; i < argc; i++)
        args[i-1] = prog_and_args[i];
    args[argc-1] = NULL;

    FILE *pipe = NULL;

    status = AuthorizationExecuteWithPrivileges(authorizationRef, prog_and_args[0], kAuthorizationFlagDefaults, args, &pipe);

    // Wait for authorized process to finish
    int stat;
    wait(&stat);

    // Release memory
    free(args);

    if (status != errAuthorizationSuccess)
    {
        //NSLog(@"Error: %d", status);
        return -1;
    }

    // The only way to guarantee that a credential acquired when you
    // request a right is not shared with other authorization instances is
    // to destroy the credential.  To do so, call the AuthorizationFree
    // function with the flag kAuthorizationFlagDestroyRights.
    // http://developer.apple.com/documentation/Security/Conceptual/authorization_concepts/02authconcepts/chapter_2_section_7.html
    status = AuthorizationFree(authorizationRef, kAuthorizationFlagDestroyRights);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc >= 2)
        return run_authorized(argc - 1, argv + 1);

    return 0;
}
