import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd


# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    username = db.execute("SELECT username FROM users WHERE id = :ids", ids = session["user_id"])
    update = db.execute("SELECT symbol, price FROM dashboard WHERE userid = :ids", ids = session["user_id"])
    rows = db.execute("SELECT COUNT(symbol) FROM dashboard WHERE userid = :ids", ids = session["user_id"])

    for i in range(rows[0]['COUNT(symbol)']):

        tmp1 = lookup(update[i]["symbol"])

        db.execute("UPDATE dashboard SET price = :prices WHERE userid = :ids AND symbol = :s",
                    ids = session["user_id"],
                    s = update[i]["symbol"],
                    prices = usd(tmp1["price"]))


    tmp = db.execute("SELECT * FROM dashboard WHERE userid = :ids", ids = session["user_id"])
    total = db.execute("SELECT SUM(total) FROM dashboard WHERE userid = :ids", ids = session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = :ids", ids = session["user_id"])
    balance = cash[0]["cash"]

    if total[0]['SUM(total)'] != None: #and float(total[0]['SUM(total)']) != 0:
        total = float(total[0]["SUM(total)"]) + balance
        return render_template("index.html", tmp=tmp, total=total, balance=balance, username=username[0]["username"])
    else:
        total = balance
        return render_template("index.html", tmp=tmp, total=balance, balance=balance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    username = db.execute("SELECT username FROM users WHERE id = :ids", ids = session["user_id"])

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide symbol.", 403)

        elif not request.form.get("shares"):
            return apology("must provide shares.", 403)

        tmp = lookup(request.form.get("symbol"))

        if tmp == None:
            return apology("Incorrect Symbol.")

        tmp1 = db.execute("SELECT * FROM dashboard WHERE symbol = :symbol AND userid = :ids", symbol = tmp["symbol"], ids = session["user_id"])

        cash = db.execute("SELECT cash FROM users WHERE id = :ids", ids = session["user_id"])

        balance = cash[0]["cash"]

        total_price = int(request.form.get("shares")) * float(tmp["price"])

        if balance < total_price:
            return apology("Insufficient funds.")

        if len(tmp1) != 1:
            db.execute("INSERT INTO dashboard values(:ids, :symbol, :shares, :price, :total)",
                        ids = session["user_id"],
                        symbol = tmp["symbol"],
                        shares = request.form.get("shares"),
                        price = tmp["price"],
                        total = float(request.form.get("shares")) * float(tmp["price"]))
        else:
            db.execute("UPDATE dashboard SET shares = shares + :nshares WHERE symbol = :nsymbol AND userid = :ids",
                        ids = session["user_id"],
                        nsymbol = tmp["symbol"],
                        nshares = int(request.form.get("shares")))

            db.execute("UPDATE dashboard SET total = total + :mtotal WHERE symbol = :msymbol AND userid = :idm",
                        idm = session["user_id"],
                        msymbol = tmp["symbol"],
                        mtotal = float(request.form.get("shares")) * float(tmp["price"]))

        db.execute("INSERT INTO history(user_id, hsymbol, hshares, hprice) values(:ids, :symbol, :shares, :price)",
                    ids = session["user_id"],
                    symbol = tmp["symbol"],
                    shares = request.form.get("shares"),
                    price = tmp["price"])


        balance = balance - total_price

        db.execute("UPDATE users SET cash = :acc WHERE id = :ids", acc = balance, ids = session["user_id"])

        flash("Bought!")
        return redirect("/")


    else:
        return render_template("buy.html", username=username[0]["username"])


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    username = db.execute("SELECT username FROM users WHERE id = :ids", ids = session["user_id"])
    tmp = db.execute("SELECT hsymbol, hshares, hprice, time FROM history JOIN users ON users.id = history.user_id WHERE id = :ids ORDER BY time desc", ids = session["user_id"])

    return render_template("history.html", tmp=tmp, username=username[0]["username"])


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    username = db.execute("SELECT username FROM users WHERE id = :ids", ids = session["user_id"])

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Please provide a symbol.")

        tmp = lookup(request.form.get("symbol"))

        if tmp == None:
            return apology("Incorrect Symbol.")

        else:
            return render_template("quote2.html", tmp=tmp, username=username)

    else:
        return render_template("quote.html", username=username[0]["username"])


@app.route("/register", methods=["GET", "POST"])
def register():

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username.", 403)

        elif not request.form.get("password"):
            return apology("must provide password.", 403)

        elif not request.form.get("cpassword"):
            return apology("must confirm password.", 403)

        rows=db.execute("SELECT COUNT(*) FROM users WHERE username = :username", username=request.form.get("username"))

        if len(rows) != 1:
            return apology("username is already taken.")
        else:
            passkey=request.form.get("password")
            cpasskey=request.form.get("cpassword")

            if passkey==cpasskey:
                db.execute("INSERT INTO users(username, hash) values(:username, :phash)",
                username=request.form.get("username"),
                phash=generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8))
                return redirect("/")
            else:
                return apology("Passwords do not match.")

    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    username = db.execute("SELECT username FROM users WHERE id = :ids", ids = session["user_id"])
    tmp = db.execute("SELECT symbol FROM dashboard WHERE userid = :ids",
                        ids = session["user_id"])

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide symbol.", 403)

        elif not request.form.get("shares"):
            return apology("must provide shares.", 403)

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        price = lookup(symbol)

        for_shares = db.execute("SELECT shares FROM dashboard WHERE symbol = :nsymbol AND userid = :ids",
                                nsymbol = symbol,
                                ids = session["user_id"])

        diff = int(for_shares[0]["shares"]) - shares

        cash = db.execute("SELECT cash FROM users WHERE id = :ids", ids = session["user_id"])

        balance = cash[0]["cash"]

        total_price = shares * float(price["price"])

        if diff < 0:
            return apology("insufficient shares.")

        elif diff == 0:
            db.execute("DELETE FROM dashboard WHERE userid = :idr AND symbol = :rsymbol",
                        idr = session["user_id"],
                        rsymbol = symbol)

        elif diff > 0:
            db.execute("UPDATE dashboard SET shares = :mshares WHERE symbol = :msymbol AND userid = :ids",
                        ids = session["user_id"],
                        msymbol = symbol,
                        mshares = diff)


            db.execute("UPDATE dashboard SET total = :ntotal WHERE symbol = :nsymbol AND userid = :idn",
                        idn = session["user_id"],
                        nsymbol = symbol,
                        ntotal = diff * int(price["price"]))

        db.execute("INSERT INTO history(user_id, hsymbol, hshares, hprice) values(:ids, :symbol, -1 * :shares, :prices)",
                    ids = session["user_id"],
                    symbol = symbol,
                    shares = request.form.get("shares"),
                    prices = price["price"])

        balance = balance + shares * float(price["price"])

        db.execute("UPDATE users SET cash = :acc WHERE id = :ids", acc = balance, ids = session["user_id"])

        flash("Sold!")
        return redirect("/")

    else:
        return render_template("sell.html", tmp=tmp, username=username[0]["username"])



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


