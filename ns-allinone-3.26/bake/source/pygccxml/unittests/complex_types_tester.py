# Copyright 2004-2008 Roman Yakovenko.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import sys
import unittest
import autoconfig
import parser_test_case

from pygccxml import utils
from pygccxml import parser
from pygccxml import declarations

class tester_t( parser_test_case.parser_test_case_t ):
    COMPILATION_MODE = parser.COMPILATION_MODE.ALL_AT_ONCE    
    def __init__(self, *args ):
        parser_test_case.parser_test_case_t.__init__( self, *args )
        self.header = 'complex_types.hpp'
        self.declarations = None
        
    def setUp(self):
        if not self.declarations:
            self.declarations = parser.parse( [self.header], self.config )
            
    def test( self ):                
        """
        This test tests presence of complex long double, float within 
        FUNDAMENTAL_TYPES map
        """
        pass 

def create_suite():
    suite = unittest.TestSuite()
    if sys.platform != 'win32':
        suite.addTest( unittest.makeSuite(tester_t))
    return suite

def run_suite():
    unittest.TextTestRunner(verbosity=2).run( create_suite() )

if __name__ == "__main__":
    run_suite()
