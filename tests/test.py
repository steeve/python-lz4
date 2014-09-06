import hashlib
import lz4
import os
import shutil
import sys
import unittest

class TestLZ4(unittest.TestCase):

    def test_random(self):
      DATA = os.urandom(128 * 1024)  # Read 128kb
      self.assertEqual(DATA, lz4.loads(lz4.dumps(DATA)))
    
    def test_file(self):
      fileName = 'src/lz4.c'
      os.mkdir('testHold')
      testNames = []
      origDigest = hashlib.md5()
      
      with open('src/lz4.c', 'rb') as lz4Orig:
        origDigest.update(lz4Orig.read())
      
      for num in range(1, 6):
        testNames.append('testHold/test.%d.lz4' % num)
      
      lz4.compressFileAdv(fileName, 9, output=testNames[0])
      lz4.compressFileAdv(fileName, 9, output=testNames[1], blockSizeID=4)
      lz4.compressFileAdv(fileName, 9, output=testNames[2], blockSizeID=7)
      lz4.compressFileAdv(fileName, 9, output=testNames[3], blockCheck=1)
      lz4.compressFileAdv(fileName, 9, output=testNames[4], streamCheck=0)
      
      for test in testNames:
        lz4.decompressFileDefault(test)
        testDigest = hashlib.md5()
        with open(test.replace('.lz4', ''), 'rb') as testFile:
          testDigest.update(testFile.read())
        self.assertEqual(origDigest.hexdigest(), testDigest.hexdigest())
        del testDigest
      shutil.rmtree('testHold')
      
if __name__ == '__main__':
    unittest.main()

