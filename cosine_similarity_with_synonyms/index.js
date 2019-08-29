// https://gist.github.com/robertknight/5410420
// modified

const __DEBUG__ = true

const log = (...args) => {
    if (__DEBUG__) {
        console.log.apply(null, args)
    }
}

const synonyms = {
    'Welcome': {
        0.8: ['Hello'],
    },
}

function findSynonym (word) {
    const result = [null, null]
    Object.keys(synonyms).forEach((desiredWord) => {
        const weightObj = synonyms[desiredWord]
        Object.keys(weightObj).forEach((weight) => {
            if (weightObj[weight].indexOf(word) !== -1) {
                result[0] = desiredWord
                result[1] = Number(weight)
                return false
            }
        })
    })
    if (result[0]) {
        log('found synonym', result)
    }
    return result
}

function termFreqMap (str, compareTo = false) {
    const words = str.split(/\s+/)
    const termFreq = {}
    words.forEach(function (word) {
        let set = false
        if (compareTo && typeof compareTo[word] === 'undefined') {
            const [word2, weight] = findSynonym(word)
            if (word2) {
                termFreq[word2] = (termFreq[word2] || 0) + weight
                set = true
            }
        }
        if (!set) {
            termFreq[word] = (termFreq[word] || 0) + 1
        }
    })
    return termFreq
}

function addKeysToDict (map, dict) {
    for (let key in map) {
        dict[key] = true
    }
}

function termFreqMapToVector (map, dict) {
    const termFreqVector = []
    for (let term in dict) {
        termFreqVector.push(map[term] || 0)
    }
    return termFreqVector
}

function vecDotProduct (vecA, vecB) {
    let product = 0
    for (let i = 0; i < vecA.length; i++) {
        product += vecA[i] * vecB[i]
    }
    return product
}

function vecMagnitude (vec) {
    let sum = 0
    for (let i = 0; i < vec.length; i++) {
        sum += vec[i] * vec[i]
    }
    return Math.sqrt(sum)
}

function cosineSimilarity (vecA, vecB) {
    return vecDotProduct(vecA, vecB) / (vecMagnitude(vecA) * vecMagnitude(vecB))
}

function textCosineSimilarity (strA, strB) {
    log('testing:', strA, strB)

    const termFreqB = termFreqMap(strB)
    const termFreqA = termFreqMap(strA, termFreqB)

    log('termFreqMap', termFreqA, termFreqB)

    const dict = {}
    addKeysToDict(termFreqA, dict)
    addKeysToDict(termFreqB, dict)

    log('dict', dict)

    const termFreqVecA = termFreqMapToVector(termFreqA, dict)
    const termFreqVecB = termFreqMapToVector(termFreqB, dict)

    log('termFreqMapToVector', termFreqVecA, termFreqVecB)

    return cosineSimilarity(termFreqVecA, termFreqVecB)
}

const result = textCosineSimilarity("Hello at banit.pl", "Welcome at banit.pl");
log("Comparison result:", result)
