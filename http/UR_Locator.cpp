/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UR_Locator.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pokpalae <pokpalae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:28:41 by pokpalae          #+#    #+#             */
/*   Updated: 2025/06/18 20:11:50 by pokpalae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "../includes/UR_Locator.hpp"

UR_Locator::UR_Locator() {

}

UR_Locator::~UR_Locator() {

}


str&    UR_Locator::getPath(void) {
    return (_path);
}   

str&    UR_Locator::getQuery(void) {
    return (_query);
}   

void    UR_Locator::setPath(str path) {
    this->_path = path; 
} 

void    UR_Locator::setQuery(str query) {
    this->_query = query;
}  
