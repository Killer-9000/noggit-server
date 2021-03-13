#include "server.h"
#include "log.h"
#include "worker_thread_mgr.h"
#include "DBC.h"

int main (int argc, const char* argv[])
{
    LOG_INFO("Starting Noggit-Server...");

    // Initialize worker threads
    //sWorkerThreadMgr.Initialize(4);

    OpenDBs();

    // Run main thread code
    sTCPServer.Inititalize(9292);

    LOG_INFO("Stopping Noggit-Server...");
    return EXIT_SUCCESS;
}
