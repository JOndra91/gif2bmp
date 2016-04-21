/**
 * File: macros.h
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#ifndef MACROS_H
#define MACROS_H

#define PACKED __attribute__((packed))

#define assert_return(assertion, returnCode) do{if(!(assertion)) return returnCode;} while(0)

#endif // MACROS_H
