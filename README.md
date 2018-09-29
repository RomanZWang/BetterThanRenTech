# BetterThanRenTech
## Running the bot
```
cd build
cmake ..
make
```

Create a file named public.txt and secret.txt. Put them in the key_store folder. This will be read by our bot to make API requests.

## Meeting 1 notes

### Resources
#### Github repo of resources
https://github.com/EliteQuant/EliteQuant
#### All Github repos of QF by stars
https://github.com/topics/quantitative-finance
#### Crypto Trading bot example interfacing with Binance
https://medium.com/@BlockchainEng/how-to-build-an-automated-cryptocurrency-trading-bot-on-binance-with-python-2cd9b5874e44
#### Quant finance strategies in academia
https://alphaarchitect.com/2015/04/01/where-to-find-cool-academic-finance-research/
#### Quant Tutorial services
Quantopian, DataCamp QA with R, Udacity

#### Books subjects to look into
C++, Python, Machine Learning, DS, Quantitative Trading, Knowledge of Learning, Scientific computing, Numerical Methods, Time Series Analysis, AI <br />
A few books are on Safari Proquest (accounts provided by university)

### Meeting Overview
In this meeting, we discussed our goals and aligned our goals.
We discussed the high-level plan for the project and what our next few steps should be. We also discussed project logistics. We also considered looking for a professor to give feedback on this project.

### Goals
Andrew - Get a project on his Resume that he can talk about<br />
Adam - Get a project on his Resume that he can talk about that makes use of his Math skills<br />
Roman - Get project on his Resume to indicate interest in Quantitative trading and gain knowledge into various fields like Low-Level language optimization, Machine Learning, Time-Series Forecasting, and trading strategy <br />
Salman -

### Project roadmap for the future
We plan on finishing the crypto bot September 21st. We plan on having a major deliverable by the end of the semester.
1. Build a crypto-trading bot from scratch using C++ so that we may be able to demonstrate we can code and understand how trading bots are written - C++ for performance and industry usage, crypto for low commission rates
2. Make good progress by reading section of a book or set of articles of our choice - we want to gain the necessary human capital for when we do develop a trading strategy
3. Develop different strategies and implement each of them, and then evaluate and test how well they work. This where we diverge and use different approaches based on our areas of expertise

### Logistics, expectations and other info
We'll be communicating through Slack.<br />
We'll be maintaining weekly development cycles, similar to the AGILE methodology for Project Management.<br />
Weekly meetings are every Friday.<br />
We'll be using C++ first and possibly Python and R in the future.<br />
We'll be treating this project professionally.

### Changes after talking with Salman

### Tasks to complete
Everyone - Come up with three fields they want to learn about when implementing this Project
Adam - Finish Crypto-Exchange APIs
Roman - Finish Crypto-bot logic after APIs are finished
Andrew - Define exit cases and conditions where we panic sell

## Meeting 2 notes

### Overview
Getting APIs set up took longer than expected. C++ libraries are less standard than python libraries and a lot harder to implement. We were able to get the major libraries like HTTP Requests and SHA256 libraries. Binance API is working which will hopefully expedite figuring out CEX.io API. Edge cases were worked on. after the meeting was over because of conflicts with the school grind. Expected finish date of crypto bot is end of September.

### Tasks from last week
Roman - finished Binance API
Adam - will finish CEX.io API a few days after the meeting
Andrew - will finish edge cases early next weekly

#### Goals
1. Complete the crypto bot
2. Do independent research and share relevant articles

##### Adam
1. Stretch: learn about reinforcement learning and attempt to implement in small scale toy application
2. “Social sentiment” try to web crawl  for what people are saying about various coins and add that as an input to our bot
3. Practical skills: learn to make fast, high quality C++ and become a god
Gain experience working with cloud services eg Amazon, Google. Use machine learning frameworks that  professionals use - TensorFlow

##### Andrew
1.  Study the impact cryptocurrency has on the environment (ex. mining and transactions take electricity i.e. energy)
2.  Learn how to better look at data and how to use outside data
3. Be knowledgeable in all these disciplines of computer science (Ex. ML, Parallel computation, HCI/NLP, etc)

##### Roman
1. Portfolio literacy - financial terminology, financial metrics, hedging, *risk analysis*
2. Programming fluency - Understand C++, Python compile level; GPU optimization
3. Use a NLP approach on web data and news
4. Use a data science approach

##### Salman
1. Learn about finance and tech combinations scalable within a software based bank system
2. Anal isis of risk
3. Finally learn ML
4. Master statistical and stochastic processes for ML/quant

## Week 3 meeting notes
### Overview
Discussed how we want to architect the trading algorithm. We want to find a service that ideally manages transactions, has virtual currency, and has access to stock prices, however if that doesn't exist, a combination of those features will be good. Last resort is to develop a portfolio from scratch or use a github repo. We want to wrap up last week's tasks and have something tangible for our strategy next week. We want to avoid bottlenecking this week.

### Tasks from last meeting
1. Crypto bot still in progress
2. Did more research

### Goals for this week
Adam - Finish CEX.io. Work on portfolio API. Work on stock trading bot logic.
Roman - Finish crypto bot logic. Work on portfolio API. Work on stock trading bot logic.
Salman - Find best trading/portfolio service early in the week. Find an interesting strategy and implement it in Python.
Andrew - Find an interesting strategy and implement it in Python. Define edge cases and panic sell conditions for stock trading bot.
