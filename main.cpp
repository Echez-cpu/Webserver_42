/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pokpalae <pokpalae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 13:54:23 by pokpalae          #+#    #+#             */
/*   Updated: 2025/07/08 13:54:27 by pokpalae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "includes/Helper.hpp"
#include "includes/Server.hpp"
#include "includes/ConfigParser.hpp"
#include <signal.h>
#include "includes/Signal_flag.hpp"


volatile sig_atomic_t stopFlag = 0;

void signalHandler(int signum) {
     (void)signum;
    stopFlag = 1;
}


int main(int argc, char** argv)
{

    signal(SIGINT, signalHandler);
    srand(time(NULL));
    
    if (argc != 2)
    {
        std::cerr << "Error: Missing configuration file.\n";
        std::cerr << "Usage: ./webserv [configuration file path]" << std::endl;
        return 1;
    }

    ConfigParser configHandler;

    try
    {
        std::string configFilePath = argv[1];

        configHandler.validateConfig(configFilePath);

    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: Unable to load configuration file - " << error.what() << std::endl;
        return -1;
    }
    try
    {
       configHandler.displayConfiguration();
    
    
    
    Server webServer(&configHandler);

    
        webServer.launchServer();
    }
    catch (const std::exception& error)
    {
        std::cerr << "Server terminated unexpectedly: " << error.what() << std::endl;
    }

    return 0;
}

