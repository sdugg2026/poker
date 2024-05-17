import './App.css';
import twoh from './playingCards/2_of_hearts.png'
import twoc from './playingCards/2_of_clubs.png'
import twod from './playingCards/2_of_diamonds.png'
import twos from './playingCards/2_of_spades.png'

import threeh from './playingCards/3_of_hearts.png'
import threec from './playingCards/3_of_clubs.png'
import threed from './playingCards/3_of_diamonds.png'
import threes from './playingCards/3_of_spades.png'

import fourh from './playingCards/4_of_hearts.png'
import fourc from './playingCards/4_of_clubs.png'
import fourd from './playingCards/4_of_diamonds.png'
import fours from './playingCards/4_of_spades.png'

import fiveh from './playingCards/5_of_hearts.png'
import fivec from './playingCards/5_of_clubs.png'
import fived from './playingCards/5_of_diamonds.png'
import fives from './playingCards/5_of_spades.png'

import sixh from './playingCards/6_of_hearts.png'
import sixc from './playingCards/6_of_clubs.png'
import sixd from './playingCards/6_of_diamonds.png'
import sixs from './playingCards/6_of_spades.png'

import sevenh from './playingCards/7_of_hearts.png'
import sevenc from './playingCards/7_of_clubs.png'
import sevend from './playingCards/7_of_diamonds.png'
import sevens from './playingCards/7_of_spades.png'

import eighth from './playingCards/8_of_hearts.png'
import eightc from './playingCards/8_of_clubs.png'
import eightd from './playingCards/8_of_diamonds.png'
import eights from './playingCards/8_of_spades.png'

import nineh from './playingCards/9_of_hearts.png'
import ninec from './playingCards/9_of_clubs.png'
import nined from './playingCards/9_of_diamonds.png'
import nines from './playingCards/9_of_spades.png'

import tenh from './playingCards/10_of_hearts.png'
import tenc from './playingCards/10_of_clubs.png'
import tend from './playingCards/10_of_diamonds.png'
import tens from './playingCards/10_of_spades.png'

import jackh from './playingCards/jack_of_hearts.png'
import jackc from './playingCards/jack_of_clubs.png'
import jackd from './playingCards/jack_of_diamonds.png'
import jacks from './playingCards/jack_of_spades.png'

import queenh from './playingCards/queen_of_hearts.png'
import queenc from './playingCards/queen_of_clubs.png'
import queend from './playingCards/queen_of_diamonds.png'
import queens from './playingCards/queen_of_spades.png'

import kingh from './playingCards/king_of_hearts.png'
import kingc from './playingCards/king_of_clubs.png'
import kingd from './playingCards/king_of_diamonds.png'
import kings from './playingCards/king_of_spades.png'

import aceh from './playingCards/ace_of_hearts.png'
import acec from './playingCards/ace_of_clubs.png'
import aced from './playingCards/ace_of_diamonds.png'
import aces from './playingCards/ace_of_spades.png'

import {useState, useEffect} from 'react'
import {Button, Spinner} from 'react-bootstrap'


const IMAGES = [
  twoh, twoc, twod, twos,
  threeh, threec, threed, threes,
  fourh, fourc, fourd, fours,
  fiveh, fivec, fived, fives,
  sixh, sixc, sixd, sixs,
  sevenh, sevenc, sevend, sevens,
  eighth, eightc, eightd, eights,
  nineh, ninec, nined, nines,
  tenh, tenc, tend, tens,
  jackh, jackc, jackd, jacks,
  queenh, queenc, queend, queens,
  kingh, kingc, kingd, kings,
  aceh, acec, aced, aces,
]



function App() {
  const [states, setStates] = useState([])
  const [selecCount, setCount] = useState(0)
  const [prob, setProb] = useState("???")
  const [submitted, setSubmit] = useState(false)

  const chars = ['2', '3', '4', '5', '6', '7', '8', '9', 'D', 'J', 'Q', 'K', 'A']
  
  useEffect(
    ()=>{
        for(let i = 0; i < 64; i++) setStates(states => [...states, "btn-inactive"])
      }
    ,[])


    const handleAddChange = (idx) => {
      
      setStates(prev => 
        states.map((o, i) => {
            if(i === idx){
              if(states[i] === "btn-inactive" && selecCount < 2){
                setCount(selecCount => selecCount + 1)
                return states[i] = "btn-active"
              }
              else if(states[i] === "btn-active"){
                setCount(selecCount => selecCount - 1)
                return states[i] = "btn-inactive"
              }
            } 
            return states[i]
        })
      )
    }

    async function handleSubmit(){
      setSubmit(true)

      let dat = []

      let curCard = 2;
      for(let i = 0; i < 64; i += 4){
        if(states[i] === "btn-active") dat.push(chars[curCard - 2] + 'h')
        if(states[i+1] === "btn-active") dat.push(chars[curCard - 2] + 'c')
        if(states[i+2] === "btn-active") dat.push(chars[curCard - 2] + 'd')
        if(states[i+3] === "btn-active") dat.push(chars[curCard - 2] + 's')

        curCard += 1
      }

      if(dat.length !== 2) return;
   
      const url = "http://127.0.0.1:5000/send_request/" + dat[0] + dat[1]

      const dt = await fetch(url)
      .then(function(resp){
        return resp.json()
      })
      .then(function(data){
        return data;
      })

      setTimeout(function(){
        

        setStates([])
        for(let i = 0; i < 64; i++) setStates(states => [...states, "btn-inactive"])
        setCount(0)
        setProb(dt)

        setSubmit(false)
        console.log(dt)
        
    

        
      }, 1000);
    }


  return (
    <div className="App">
    
      <div className="allcards">
        
        {
          IMAGES.map( (key,index) => (
            <button className={states[index]} key={index} onClick={(e) => handleAddChange(index) }>
              <img className={"Card"} src={key}/>
            </button>
          ))
        }
      </div>
        
      <div className="label">
        <h1> How good is your hand? </h1>
        <h2> (choose two cards) </h2>

        
        {
          !submitted? 
          <Button variant="primary" onClick={handleSubmit}> calculate </Button> 
          : 
          <Button variant="primary" disabled>
            <Spinner
              as="span"
              animation="border"
              size="sm"
              role="status"
              aria-hidden="true"
            />
            <span className="visually-hidden">Loading...</span>
          </Button>
        }
        
        
        <h2> Probability of winning: {prob} </h2>
      </div>
      
    </div>
  );
}

export default App;
