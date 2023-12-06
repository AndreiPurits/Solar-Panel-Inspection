from dash import Dash, dcc, html, Input, Output, State
import pandas as pd
import json
import plotly.graph_objects as go
import plotly.express as px
import cv2
import numpy as np
import imutils
import base64
from io import BytesIO

app = Dash(__name__)

df = pd.read_csv('output_data.csv')

app.layout = html.Div([
    html.H4('Interactive Defect Analysis'),
    
    # Image Display Section
    html.Div([
        html.Div([
            dcc.Graph(id="defect-image"),
        ], className="six columns"),
    ], className="row"),
    
    # Scatter Plot Section
    html.Div([
        html.Div([
            dcc.Graph(id="scatter-plot"),
            html.P("Filter by Parameters:"),
            html.Label("Select min_contour_area:"),
            dcc.RangeSlider(
                id='min-contour-area-slider',
                min=int(df['min_contour_area'].min()),
                max=int(df['min_contour_area'].max()),
                step=1,
                value=[int(df['min_contour_area'].min()), int(df['min_contour_area'].max())],
                marks={i: str(i) for i in range(int(df['min_contour_area'].min()), int(df['min_contour_area'].max()) + 1)}
            ),
            html.Label("Select max_contour_area:"),
            dcc.RangeSlider(
                id='max-contour-area-slider',
                min=int(df['max_contour_area'].min()),
                max=int(df['max_contour_area'].max()),
                step=1,
                value=[int(df['max_contour_area'].min()), int(df['max_contour_area'].max())], 
                marks={i: str(i) for i in range(int(df['max_contour_area'].min()), int(df['max_contour_area'].max()) + 1)}
            ),
            html.Label("Select iterationz:"),
            dcc.RangeSlider(
                id='iterationz-slider',
                min=int(df['iterationz'].min()),
                max=int(df['iterationz'].max()),
                step=1,
                value=[int(df['iterationz'].min()), int(df['iterationz'].max())], 
                marks={i: str(i) for i in range(int(df['iterationz'].min()), int(df['iterationz'].max()) + 1)}
            ),
            html.Label("Select threshold_value:"),
            dcc.RangeSlider(
                id='threshold-value-slider',
                min=int(df['threshold_value'].min()),
                max=int(df['threshold_value'].max()),
                step=1,
                value=[int(df['threshold_value'].min()), int(df['threshold_value'].max())], 
                marks={i: str(i) for i in range(int(df['threshold_value'].min()), int(df['threshold_value'].max()) + 1)}
            ),
            html.Div(id='data-count'),
        ], className="six columns"),
    ], className="row"),
])

@app.callback(
    Output("scatter-plot", "figure"), 
    Output("data-count", "children"),
    Input("min-contour-area-slider", "value"),
    Input("max-contour-area-slider", "value"),
    Input("iterationz-slider", "value"),
    Input("threshold-value-slider", "value"))
def update_scatter_plot(min_contour_area_range, max_contour_area_range, iterationz_range, threshold_range):
    filtered_data = df[
        (df['min_contour_area'] >= min_contour_area_range[0]) & (df['min_contour_area'] <= min_contour_area_range[1]) &
        (df['max_contour_area'] >= max_contour_area_range[0]) & (df['max_contour_area'] <= max_contour_area_range[1]) &
        (df['iterationz'] >= iterationz_range[0]) & (df['iterationz'] <= iterationz_range[1]) &
        (df['threshold_value'] >= threshold_range[0]) & (df['threshold_value'] <= threshold_range[1])
    ]
    dot_sizes = filtered_data['average_area_data'] * filtered_data['defects_data']

    fig = px.scatter(
        filtered_data, 
        x='average_area_data', y='defects_data',
        hover_data=['min_contour_area', 'max_contour_area', 'iterationz', 'threshold_value'],
        labels={'average_area_data': 'Average Area', 'defects_data': 'Defects'},
        title="Defect Analysis",
    )

    fig.update_traces(marker=dict(size=dot_sizes * 0.03))
    data_count = len(filtered_data)     
    count_text = f"Number of Shown Dots: {data_count}"

    return fig, count_text 

@app.callback(
    Output("defect-image", "figure"),
    Input("scatter-plot", "selectedData")
)
def update_defect_image(selectedData):
    if selectedData is None or len(selectedData['points']) == 0:
        return go.Figure()

    selected_point = selectedData['points'][0]
    min_contour_area = selected_point['customdata'][0]
    max_contour_area = selected_point['customdata'][1]
    iterationz = selected_point['customdata'][2]
    threshold_value = selected_point['customdata'][3]

    result = process_image(threshold_value, iterationz, max_contour_area, min_contour_area)

    _, encoded_image = cv2.imencode('.png', result)
    image_data = base64.b64encode(encoded_image).decode('utf-8')

    return {
        'data': [{
            'x': [0, 1],
            'y': [0, 1],
            'mode': 'markers',
            'marker': {
                'opacity': 0,
                'size': 0
            },
            'text': [f"min_contour_area: {min_contour_area}<br>max_contour_area: {max_contour_area}<br>iterationz: {iterationz}<br>threshold_value: {threshold_value}"],
            'hoverinfo': 'text',
        }],
        'layout': {
            'images': [{
                'source': f"data:image/png;base64,{image_data}",
                'x': 0,
                'y': 1,
                'xref': 'x',
                'yref': 'y',
                'sizex': 1,
                'sizey': 1,
                'xanchor': 'left',
                'yanchor': 'top',
            }],
            'xaxis': {
                'range': [0, 1],
                'showgrid': False,
                'zeroline': False,
                'showticklabels': False,
            },
            'yaxis': {
                'range': [0, 1],
                'showgrid': False,
                'zeroline': False,
                'showticklabels': False,
            },
            'showlegend': False,
        }
    }

def process_image(threshold_value, iterations_value, max_contour_area, min_contour_area):
    img = cv2.imread("defect_frame_12.png")
    img = cv2.normalize(img, img, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    ret, thresh = cv2.threshold(img_gray, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    kernel = np.ones((3, 28), np.uint8)
    opening = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel, iterations_value)
    sure_bg = cv2.dilate(opening, kernel, iterations=3)
    dist_transform = cv2.distanceTransform(opening, cv2.DIST_L2, 5)
    ret, sure_fg = cv2.threshold(dist_transform, 0.001 * dist_transform.max(), 255, 0)
    sure_fg = np.uint8(sure_fg)
    unknown = cv2.subtract(sure_bg, sure_fg)
    ret, markers = cv2.connectedComponents(sure_fg)
    markers = markers + 1
    markers[unknown == 255] = 0
    markers = cv2.watershed(img, markers)
    mask = np.where(markers != 1, 1, 0).astype(np.uint8)
    result2 = cv2.bitwise_and(img, img, mask=mask)

    edges = cv2.Canny(image=result2, threshold1=29, threshold2=30)
    cnts = cv2.findContours(edges, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)
    cnts = imutils.grab_contours(cnts)
    cnts = sorted(cnts, key=cv2.contourArea, reverse=True)

    for c in range(len(cnts)):
        [x, y, w, h] = cv2.boundingRect(cnts[c])
        if w * h < 50000:
            cv2.rectangle(opening, (x, y), (x + w, y + h), (255, 255, 255), -1)
    opening = cv2.morphologyEx(opening, cv2.MORPH_OPEN, kernel=np.ones((5, 5), np.uint8))
    cnts = cv2.findContours(opening, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)
    cnts = imutils.grab_contours(cnts)
    cnts = sorted(cnts, key=cv2.contourArea, reverse=True)

    if len(cnts) >= 1:
        rot_rect = cv2.minAreaRect(cnts[0])
        (x, y), (w, h), angle = rot_rect
        box = cv2.boxPoints(rot_rect)
        box = np.int0(box)

        roi = img[min(box[:, 1]):max(box[:, 1]), min(box[:, 0]):max(box[:, 0])]

        img_gray = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
        imgу = cv2.normalize(img_gray, img_gray, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
        _, binary_mask = cv2.threshold(imgу, threshold_value, 255, cv2.THRESH_BINARY)
        contours, _ = cv2.findContours(binary_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        selected_contours = []
        if len(contours) != 0:
            for contour in contours:
                area = cv2.contourArea(contour)
                if min_contour_area < area < max_contour_area:
                    selected_contours.append(contour)
            result = np.copy(roi)

            for contour in selected_contours:
                x, y, w, h = cv2.boundingRect(contour)
                cv2.drawContours(result, [contour], -1, (0, 255, 0), thickness=cv2.FILLED)
                cv2.rectangle(result, (x, y), (x + w, y + h), (0, 0, 255), 2)
                cv2.putText(result, "Defect", (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    return result

if __name__ == "__main__":
    app.run_server(debug=True)