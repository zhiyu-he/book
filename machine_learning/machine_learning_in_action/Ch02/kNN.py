# -*- coding: utf-8 -*-

'''
    kNN step:
    1) calculate input data distance over all labled data
    2) then choose top k item by sorted distance
    3) max(k-items) appear lable as input data's label

'''

import numpy as np
import operator
import os

'''
label2Num = {
        'didntLike': 1,
        'largeDoses': 2,
        'smallDoses': 3,
        }
'''

def createDataSet():
    group = np.array([[1.0, 1.1],
                        [1.0, 1.0],
                        [0,0],
                        [0, 0.1]])
    labels = ['A', 'A', 'B', 'B']
    return group, labels


def classify0(inX, dataSet, labels, k):
    dataSetSize = dataSet.shape[0] # shape return N, D, N means size, D means dimension
    diffMat = np.tile(inX, (dataSetSize, 1)) - dataSet
    sqDiffMat = diffMat ** 2
    sqDistances = sqDiffMat.sum(axis=1)
    distances = sqDistances ** 0.5
    sortedDistIndicies = distances.argsort()
    classCount={}
    for i in range(k):
        voteIlabel = labels[sortedDistIndicies[i]]
        classCount[voteIlabel] = classCount.get(voteIlabel, 0) + 1
    sortedClassCount = sorted(classCount.iteritems(), key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]

# https://www.codecademy.com/articles/normalization
def minmaxAutoNorm(dataSet):
    minVals = dataSet.min(0)
    maxVals = dataSet.max(0)
    ranges = maxVals - minVals
    normDataSet = np.zeros(np.shape(dataSet))

    m = dataSet.shape[0]
    normDataSet = dataSet - np.tile(minVals, (m, 1))
    normDataSet = normDataSet/np.tile(ranges, (m, 1))
    return normDataSet, ranges, minVals


def file2matrix(filename):
    with open(filename) as f:
        arrayOfLines = f.readlines()
        numberOfLines = len(arrayOfLines)
        returnMat = np.zeros((numberOfLines, 3))
        classLabelVector = []
        index = 0
        for line in arrayOfLines:
            line = line.strip()
            listFromLine = line.split('\t')
            returnMat[index, :] = listFromLine[0:3]
            classLabelVector.append(int(listFromLine[-1]))
            index += 1
        return returnMat, classLabelVector


def datingClassTest():
    hoRatio = 0.10
    datingDataMat, datingLabels = file2matrix('datingTestSet2.txt')
    norMat, ranges, minVals = minmaxAutoNorm(datingDataMat)
    m = norMat.shape[0]
    numTestCnt = int(m * hoRatio)
    errCnt = 0
    for i in range(numTestCnt):
        classifyRes = classify0(norMat[i, :], norMat[numTestCnt:m, :], datingLabels[numTestCnt:m], 20)
        if classifyRes is not datingLabels[i]:
            errCnt += 1
    print "errCnt: %d testCnt: %d total: %d errRatio: %f" % (errCnt, numTestCnt, m, errCnt/float(numTestCnt))


def img2vector(filename):
    returnVec = np.zeros((1, 1024))
    with open(filename) as f:
        for i in range(32):
            lineStr = f.readline()
            for j in range(32):
                returnVec[0, 32*i+j] = int(lineStr[j])
    return returnVec



def handwritingClassTest():
    hwLabels = []
    trainingFileList = os.listdir('./digits/trainingDigits')
    m = len(trainingFileList)
    trainingMat = np.zeros((m,1024))
    for i in range(m):
        fileNameStr = trainingFileList[i]   # 8_121.txt
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        hwLabels.append(classNumStr)
        trainingMat[i,:] = img2vector('./digits/trainingDigits/%s' % fileNameStr)
    
    errCnt = 0.0
    testFileList = os.listdir('./digits/testDigits')
    mTest = len(testFileList)
    for i in range(mTest):
        fileNameStr = testFileList[i]
        fileStr = fileNameStr.split('.')[0]
        testNumStr = int(fileStr.split('_')[0])
        vector4Test = img2vector('./digits/testDigits/%s' % fileNameStr)
        classifierResult = classify0(vector4Test, trainingMat, hwLabels, 10)
        if classifierResult != testNumStr:
            errCnt += 1
    print "total(mTest): %f errCnt: %f ratio: %f" % (mTest, errCnt, errCnt/mTest)
        

if __name__ == '__main__':
    groups, labels = createDataSet()
    # test-case 1
    print classify0([0,0], groups, labels, 3)

    # test-case 2
    groups, labels = file2matrix('datingTestSet2.txt')
    
    # plot picture
    '''
    import matplotlib
    import matplotlib.pyplot as plt
    fig = plt.figure()
    ax = fig.add_subplot(111)
    #ax.scatter(groups[:, 1], groups[:, 2])
    ax.scatter(groups[:, 1], groups[:, 2], 15.0 * np.array(labels), 15.0 * np.array(labels))
    plt.show()

    print minmaxAutoNorm(groups)
    '''
    datingClassTest()
    handwritingClassTest()

