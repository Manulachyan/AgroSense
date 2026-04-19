import streamlit as st
import random
import pandas as pd
import joblib

# Load model
yield_model = joblib.load("crop_yield_model.pkl")
yield_features = joblib.load("yield_features.pkl")

# Fertilizer logic
def recommend_fertilizer(sensor):
    if sensor["Nitrogen"] < 40:
        return "Urea"
    elif sensor["Phosphorous"] < 40:
        return "DAP"
    elif sensor["Potassium"] < 40:
        return "Potassium fertilizer"
    else:
        return "20-20"

# Yield prediction
def predict_yield(sensor):
    data = {
        "average_rain_fall_mm_per_year": random.randint(800, 1500),
        "avg_temp": sensor["Temperature"],
        "pesticides_tonnes": random.randint(100, 500),
        "Item": sensor["Crop Type"],
        "Area": "India"
    }

    df = pd.DataFrame([data])
    df = pd.get_dummies(df)
    df = df.reindex(columns=yield_features, fill_value=0)

    return yield_model.predict(df)[0]

# UI
st.title("🌾 Smart Agriculture System")

temp = st.slider("Temperature", 20, 40)
nitrogen = st.slider("Nitrogen", 0, 100)
phosphorous = st.slider("Phosphorous", 0, 100)
potassium = st.slider("Potassium", 0, 100)

crop = st.selectbox("Crop Type", ["Rice", "Wheat", "Maize"])

if st.button("Predict"):

    sensor = {
        "Temperature": temp,
        "Nitrogen": nitrogen,
        "Phosphorous": phosphorous,
        "Potassium": potassium,
        "Crop Type": crop
    }

    fert = recommend_fertilizer(sensor)
    yld = predict_yield(sensor)

    st.success(f"Recommended Fertilizer: {fert}")
    st.success(f"Predicted Yield: {yld}")